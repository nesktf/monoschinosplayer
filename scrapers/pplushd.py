#!/usr/bin/env python
import sys
import requests as curl
import re as regex
import json

def popular():
    url = "https://pelisplushd.is/series"
    html=curl.get(url).content
    wea = regex.search(r"(?<=\"Posters\").*?(?=example)", str(html)).group(0).split("<a")
    wea.pop(0)
    urls = []
    for i in wea:
        urls.append(
            regex.search(r"(?<=href=\").*?(?=\" class)",i).group(0)
        )
    return urls

def search(query):
    url = "https://pelisplushd.is/search?s="+query
    html = curl.get(url).content
    wea = regex.search(r"(?<=\"Posters\").*?(?=content-footer)", str(html)).group(0).split("<a")
    wea.pop(0)
    urls = []
    for i in wea:
        urls.append(
            regex.search(r"(?<=href=\").*?(?=\" class)",i).group(0)
        )
    return urls

def parseEp(url):
    html = curl.get(url).content
    wea = regex.search(r"(?<=tab-content).*?(?=class=\"clear\")",str(html)).group(0).split("tabpanel")
    wea.pop(0)
    urls=[]
    for i in wea:
        wea2 = i.split("<a")
        wea2.pop(0)
        for j in wea2:
            urls.append(
                regex.search(r"(?<=href=\").*?(?=\" class)",j).group(0)
            )
    return urls

def streamParser(url):
    html = curl.get(url).content
    wea = regex.search(r"(?<=slug).*?(?=script)",str(html)).group(0).split("video")
    wea.pop(0)
    urls = []
    for i in wea:
        urls.append(
            regex.search(r"(?<=').*?(?=\\)",i).group(0)
        )
    return urls

# SBFull extractor
def sbfull(id): # Named SBFast in website
    # TODO: Fix 403 forbidden in mpv
    url="https://sbfull.com/sources48/625a364258615242766475327c7c{hex}7c7c4761574550654f7461566d347c7c73747265616d7362"
    hexarr=['0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F']

    byt=id.encode('utf-8') 
    hexchars = [None,]*(len(byt)*2)
    for i in range(len(byt)):
        v = int(byt[i]) & 0xFF
        hexchars[i*2] = hexarr[v >> 4]
        hexchars[(i*2) + 1] = hexarr[v&0x0F]
    
    finalhex=''.join(str(x) for x in hexchars) # convert hexchars to string
    reqst = curl.get(url.format(hex=finalhex),headers={"watchsb": "sbstream"})
    print(reqst.headers)
    print(reqst.content)
    return json.loads(reqst.content)["stream_data"]["file"]
    #return json.loads(curl.get(url.format(hex=finalhex),headers={"watchsb": "sbstream"}).content)["stream_data"]["file"]

# Fembed extractor
def fembed(url): # Named PlusTo in website
    wea=json.loads(curl.post(url.replace("/v/", "/api/source/")).content)
    dict={}
    for i in wea["data"]:
        dict[i["label"]] = i["file"]
    return dict

#streamParser("https://pelisplushd.is/serie/coraje-el-perro-cobarde/temporada/1/capitulo/1")
print(streamParser(parseEp(search("coraje")[0])[0]))

#print(fembed("https://owodeuwu.xyz/v/24571a26yrkpl71")['720p']) # https://owodeuwu.xyz/v/24571a26yrkpl71 : https://api.mycdn.moe/furl.php?id=24571a26yrkpl71

#print(sbfull("2fhifolig89f")) # https://sbfull.com/e/2fhifolig89f : https://api.mycdn.moe/sblink.php?id=2fhifolig89f
