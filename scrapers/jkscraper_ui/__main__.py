#!/usr/bin/env python
import requests as curl
import re as regex
import jk_search, jk_scrapers, sys, getopt

"""
class strColors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
"""

def intSelection(max, min = 0):
    n = input()
    if n == "":
        return min
    elif (int(n) > max) or (int(n) < min):
        print("Numero inválido")
        exit(1)
    else:
        return int(n)

def snSelection(defValue=True):
    x = input()
    if (x == "s") or (x == "S"):
        return True
    elif (x == "n") or (x == "N"):
        return False
    else:
        return defValue

def UIMain():
    print("""¿Que deseas hacer?
1)  Buscar Animes
2)  Ingresar URL de un anime
3)  Ingresar URL del episodio de un anime
0)  Salir
=> """, end="")

    promptPosition = intSelection(3)
    if promptPosition == 0:
        exit(1)
    dataFlag = False
    epFlag = False

    if promptPosition == 1:
        print("Ingresa el nombre del anime a buscar:")
        print("=> ", end = "")
        animeName = str(input())
        animeData = jk_search.getSearchJson(animeName)['animes']
        if len(animeData) == 0:
            print("No se han encontdao animes con ese nombre :(")
            exit(1)
        print("Animes encontrados:")
        for i in range(len(animeData)):
            print(str(i+1)+") "+animeData[i]['title'])
            if "altertitles" in animeData[i].keys():
                try:
                    if type(animeData[i]['altertitles']) is dict:
                        forIteration = animeData[i]['altertitles'].keys()
                    else:
                        forIteration = range(len(animeData[i]['altertitles']))
                    
                    for j in forIteration:
                        print("     ["+animeData[i]['altertitles'][j]['title']+"]")
                except:
                    print("     "+animeData[i]['altertitles'])

        print("Ingresa el id del anime que deseas ver [1-"+str(len(animeData))+"]:")
        print("=> ", end="")

        animeSel = intSelection(len(animeData), 1)-1
        animeSlug = animeData[animeSel]['slug']
        animeId = animeData[animeSel]['id']
        dataFlag = True
        promptPosition += 1
    
    if promptPosition == 2:
        if not dataFlag:
            print("Ingresa la URL del anime:")
            print("=> ", end="")
            animeSel = input()
            animeSlug = regex.search(r"(?<=.net/).*?(?=/)", animeSel).group(0)
            animeId = jk_search.getAnimeId(animeSel)

        epCount = 0
        for i in range(jk_search.getJsonPageCount(animeSlug)):
            epInfoJson = jk_search.getEpisodesJson(animeId, i+1)
            epCount += len(epInfoJson)
        print("Ingresa el numero de episodio que deseas ver [1-"+str(epCount)+"]:")
        print("=> ", end="")
        epSel = intSelection(epCount, 1)
        epData = jk_search.getServers("https://jkanime.net/"+animeSlug+"/"+str(epSel)+"/")
        epFlag = True
        promptPosition += 1

    if promptPosition == 3:
        if not epFlag:
            print("Ingresa la URL del episodio:")
            print("=> ", end="")
            epSel = input()
            epData = jk_search.getServers(epSel)

        print("¿Deseas seleccionar un servidor? [s/N]")
        print("=> ", end="")
        if snSelection(False):
            print("Servidores disponibles:")
            for i in range(len(epData)):
                print(str(i+1)+") "+epData[i].name+" Lang: "+jk_scrapers.languages[int(epData[i].lang)], end="")
                if epData[i].name not in jk_scrapers.supported:
                    print(" (unsupported)")
                else:
                    print("")
            print("=> ", end="")
            servSel = intSelection(len(epData), 1)-1
            if epData[servSel].name not in jk_scrapers.supported:
                print("Servidor no soportado :(")
                exit(1)
            videoData = epData[servSel]
        else:
            for i in range(len(epData)):
                if epData[i].name == jk_scrapers.supported[0]:
                    videoData = epData[i]
                    break
    
    print("""¿Que deseas hacer con el episodio seleccionado?
1)  Cargar stream en mpv
2)  Descargar episodio
3)  Generar M3U del stream
0)  Salir""")
    print("=> ", end="")

    userSel = intSelection(3)
    if userSel == 0:
        exit(1)
    streamURL = jk_scrapers.getStream(videoData)

    if userSel == 1:
        import os
        print("Abriendo mpv...")
        os.system("mpv \""+str(streamURL+"\""))
    else:
        print(streamURL)
        print("No implementado")
        exit(1)

    """
    print(strColors.BOLD+strColors.HEADER+"Ingresa la URL de tu video: "+strColors.ENDC, end="")
    url = str(input())
    servers = scrapers.getServers(url)

    print(strColors.BOLD+strColors.HEADER+"Servidores encontrados:"+strColors.ENDC)
    for i in range(len(servers)):
        if servers[i].name in scrapers.supported:
            print(strColors.OKBLUE+"\t"+servers[i].id+strColors.OKGREEN+" => "+strColors.OKBLUE+servers[i].name)
    
    print(strColors.BOLD+strColors.HEADER+"Ingresa la id de un server: "+strColors.ENDC, end="")
    selId = int(input())-1
    streamURL = scrapers.getStream(servers[selId])

    print(strColors.BOLD+strColors.HEADER+"Abriendo mpv..."+strColors.ENDC)
    os.system("mpv \""+str(streamURL)+"\"")
    """


def parseFlags(argv):
    def printHelp():
        print(
"""Posibles argumentos:
    -h          => Mostrar esta lista
    -s          => Usar interfaz de usuario
    -u <url>    => Obtener stream de la url indicada
    
Lista de prioridad para los streams:
    Nozomi > Desu > Okru""")
    try:
        opts, args = getopt.getopt(argv,"hsu:",["stream="])
    except getopt.GetoptError:
        print("ERROR: Invalid arguments")
        printHelp()
        exit(1)
    
    for opt, arg in opts:
        if opt == '-h':
            printHelp()
            sys.exit()
        elif opt == "-s":
            return True
        elif opt == "-u":
            return arg

if __name__ == "__main__":
    flagValue = parseFlags(sys.argv[1:])

    if(isinstance(flagValue, str)):
        print(flagValue)
    else:
        try:
            UIMain()
        except KeyboardInterrupt:
            sys.exit("\nKeyboard Interrupt")
