#!/usr/bin/env python
import sys
import requests as curl
import re as regex

#def sanitizeHtml(html):
#    return regex.sub(r"[\n\t\s]*", "", str(html).replace("\\n", "").replace("\\t", ""))

def search(query):
    url = "https://jkanime.net/buscar/"+query.replace(" ", "_")+"/"
    reqHeaders = {
        "referer": "https://jkanime.net"
    }
    animus = []
    c = 0
    while True:
        c += 1
        curUrl = url + str(c)
        #html = sanitizeHtml(curl.get(curUrl, headers=reqHeaders).content)
        html = curl.get(curUrl, headers=reqHeaders).content
        wea = regex.search(r"(?<=Filtrar\<\/button>).*?(?=navigation)", str(html)).group(0).split("\"anime__item\"")
        if (len(wea) == 1):
            break
        else:
            print(c)
            wea.pop(0)
            for i in range(len(wea)):
                slug = regex.search(r"(?<=jkanime.net\/).*?(?=\/)", wea[i]).group(0)
                title = regex.search(r"(?<=\"title\"\>).*?(?=\<\/div\>)", wea[i]).group(0)
                animus.append(title+"=>"+slug)
    return animus

print(search(sys.argv[1]))
