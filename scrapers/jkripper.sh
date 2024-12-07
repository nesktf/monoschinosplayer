#!/bin/bash

if [[ -n $1 ]]; then
	url="$1"
else
	echo "No url provided"
	exit 1
fi

html=$(curl -s "$url")

getServers() {
	# Output format:
	# <ServerName>|<language_id>|<stream_id>
	# Nozomi|1|1
	scrapedServers=$(printf "%s" "$html"  | tr -d '\n' | grep -oP '(?<=Servidores:</b>).*?(?=</div>)' | sed 's/<\/li>/\n&/g' | sed -e 's/<\/li>//g;s/[[:space:]]//g;/^$/d')
	for i in $(seq $(wc -l <<<"$scrapedServers")); do
		current_url=$(awk ' BEGIN{ RS = ""; FS = "\n" } {print $'$i'}' <<< "$scrapedServers")
		id=$(grep -oP '(?<=data-id=").*(?="class)' <<< "$current_url")
		name=$(grep -oP '(?<=>).*(?=</a>)' <<< "$current_url")
		lang=$(grep -oP '(?<=lg_).*(?=mode)' <<< "$current_url")
		serverList+=("$(printf "%s|%s|%s" "$name" "$lang" "$id")")
		unset current_url id name lang
	done
	unset scrapedServers
}

function getStream() {
# TODO Añadir soporte para otros servers que no sean Nozomi
	streamIndex="$1"
	scrapedStreams=$(printf "%s" "$html" | tr -d '\n' | grep -oP '(?<=var video = \[\];).*?(?=var video_data)' | sed 's/\;/\n&/g' | sed -e 's/\;//g;s/[[:space:]]//g;/^$/d')
	selectedStream=$(grep -oP '(?<=video\['$streamIndex'\]=).*(?=</iframe>)'<<< "$scrapedStreams")
	streamUrl=$(grep -oP '(?<=src=").*(?="width)'<<<"$selectedStream")
	firstHash=$(curl -s "$streamUrl" | grep -oP '(?<=value=").*(?=")')
	unset streamIndex scrapedStreams selectedStream streamUrl

	apiHash=$(curl -v \
		-d "data=$firstHash" \
		-H "Content-Type: application/x-www-form-urlencoded" \
		-H "Origin: https://jkanime.net" \
		-H "Authority: jkanime.net" \
		"https://jkanime.net/gsplay/redirect_post.php" 2>&1 | grep location:)
	apiHash=${apiHash##*\#}
	
# 	printf "Video API hash:\n$apiHash\n\n"
	apiResponse=$(curl -s \
		-d "v=$apiHash" \
		"https://jkanime.net/gsplay/api.php")

	streamUrl=$(jq -r '.[]' <<<"$apiResponse")
	printf "\nStream URL:\n$streamUrl\n"
	unset firstHash apiHash apiResponse streamUrl
}

listServers() {
# TODO *Insertar selector con fzf aqui*
	getServers
	printf "Lista de servidores:\n"
	for i in "${serverList[@]}"; do
		while IFS="|" read -r name language id; do
			printf "Server: %s\t" "$name"
			printf "Idioma: %s\t" "$language"
			printf "Server ID: %s\n" "$id"
		done<< EOF
$i
EOF
	done
	unset name language id
	printf "\nSeleccionando Nozomi...\n"
	streamId=1
}

listServers
getStream "$streamId"
#mpv "$streamUrl"
