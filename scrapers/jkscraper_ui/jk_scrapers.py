import requests as curl
import re as regex
import jk_functions

supported = ("Nozomi", "Desu", "Okru")
languages = [None, "Jap. Sub.", None, "Esp. Lat."]


def nozomiStream(url, maxRetries = 3):
    import json
    # First hash request
    reqHeaders = {
        "referer": "https://jkanime.net/"
    }
    html = jk_functions.sanitizeHtml(curl.get(url, headers=reqHeaders).content)
    firstHash = regex.search(r"(?<=value=\").*?(?=\")", html).group(0)

    # Api hash request
    reqHeaders = {
        "referer": "https://jkanime.net/",
        "Content-Type": "application/x-www-form-urlencoded",
        "Origin": "https://jkanime.net",
        "Authority": "jkanime.net"
    }
    postData = {
        "data": firstHash
    }
    html = curl.post("https://jkanime.net/gsplay/redirect_post.php", headers=reqHeaders, data=postData)
    apiHash = html.url.split("#", 1)[1]

    # Video file request
    reqHeaders = {
        "referer": "https://jkanime.net/"
    }
    postData = {
        "v": apiHash
    }
    #apiResponse = curl.post("https://jkanime.net/gsplay/api.php", headers=reqHeaders, data=postData)

    for i in range(maxRetries):
        apiResponse = curl.post("https://jkanime.net/gsplay/api.php", headers=reqHeaders, data=postData)
        try:
            jsonResponse = json.loads(apiResponse.content)
            returnValue = jsonResponse["file"]
            if (returnValue == None):
                continue
            else:
                break
        except:
            continue
            #returnValue = None
    return returnValue

def desuStream(url):
    try:
        html = jk_functions.sanitizeHtml(curl.get(url).content)
        hlsUrl = regex.search(r"(?<=swarmId:\\\').*?(?=\\\')", html).group(0)
        return hlsUrl
    except:
        return None

def okruStream(url):
    try:
        html = jk_functions.sanitizeHtml(curl.get(url).content)
        redirect = regex.search(r"(?<=src=\").*?(?=\")", html).group(0)
        html = jk_functions.sanitizeHtml(curl.get(redirect).content).replace("&quot;", "\"")
        hlsUrl = regex.search(r"(?<=hlsManifestUrl\\\\\":\\\\\").*?(?=\\\\\")", html).group(0).replace("\\\\\\\\u0026","&")
        return hlsUrl
    except:
        return None

def getStream(serverData):
    serverData.url = "https://jkanime.net"+serverData.url
    if serverData.name == "Nozomi":
        return nozomiStream(serverData.url)
    elif serverData.name == "Desu":
        return desuStream(serverData.url)
    elif serverData.name == "Okru":
        return okruStream(serverData.url)
    else:
        print("\""+serverData.name+"\" no soportado :(")
        exit(1)
        return None
