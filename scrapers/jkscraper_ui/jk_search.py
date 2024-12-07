import requests as curl
import re as regex
import json, jk_functions

def getSearchJson(searchQuery):
    getParameters = {
        "q": searchQuery
    }
    return json.loads(curl.get("https://jkanime.net/ajax/ajax_search/", params=getParameters).content)

def getJsonPageCount(animeSlug):
    html = jk_functions.sanitizeHtml(curl.get("https://jkanime.net/"+animeSlug+"/").content)
    return len(regex.search(r"(?<=\"anime__pagination\">).*?(?=</div>)", html).group(0).split("</a>"))-1


def getEpisodesJson(animeId, jsonPage):
    return json.loads(curl.get("https://jkanime.net/ajax/pagination_episodes/"+str(animeId)+"/"+str(jsonPage)+"/").content)

def getAnimeId(animeUrl):
    html = jk_functions.sanitizeHtml(curl.get(animeUrl).content)
    return regex.search(r"(?<=data-anime=\").*?(?=\")", html).group(0)

def getServers(url):
    class ServerInstance():
        def __init__(self, scrapedName, scrapedId, scrapedUrl, scrapedLang):
            self.name = scrapedName
            self.id = scrapedId
            self.url = scrapedUrl
            self.lang = scrapedLang
    
    html = jk_functions.sanitizeHtml(curl.get(url).content)
    servIds = regex.search(r"(?<=Servidores:</b>).*?(?=</div>)", html).group(0).split("</li>")
    servUrls = regex.search(r"(?<=varvideo=\[\];).*?(?=</div>)", html).group(0).split("</iframe>")
    servList = []

    for i in range(len(servIds)-1):
        servList.append(ServerInstance(
            regex.search(r"(?<=>).*?(?=<)", servIds[i]).group(0),           # name
            regex.search(r"(?<=data-id=\").*?(?=\")", servIds[i]).group(0), # id
            regex.search(r"(?<=src=\").*?(?=\")", servUrls[i]).group(0),    # url
            regex.search(r"(?<=lg_).*?(?=m)", servIds[i]).group(0)          # lang 
        ))

    return servList