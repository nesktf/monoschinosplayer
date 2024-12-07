#!/usr/bin/env python
import re as regex
import requests as curl
import sys

def sanitizeHtml(html):
    return regex.sub(r"[\n\t\s]*", "", str(html).replace("\\n", "").replace("\\t", ""))
    
def okruStream(url):
    try:
        if "embed" not in url:
            url = "http://ok.ru/videoembed/" + url.split("/")[-1]
        html = sanitizeHtml(curl.get(url).content).replace("&quot;", "\"")
        hlsUrl = regex.search(r"(?<=hlsManifestUrl\\\\\":\\\\\").*?(?=\\\\\")", html).group(0).replace("\\\\\\\\u0026","&")
        return hlsUrl
    except:
        return None

print(okruStream(sys.argv[1]))
