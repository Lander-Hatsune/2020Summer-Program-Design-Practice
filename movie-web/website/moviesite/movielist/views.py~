from django.shortcuts import render
from django.http import HttpResponse

import json
import re
import time

# Create your views here.

movies = json.loads(open('./data/jsons/movies.json').read())
actors = json.loads(open('./data/jsons/actors.json').read())

movie_titles = list(movies.keys())
actor_names = list(actors.keys())


def index(request, page=1):
    pages = filter(lambda x: x > 0 and x < 52, list(range(page - 2, page + 3)))
    context = {
        'movies': movie_titles[(page - 1) * 20:page * 20],
        'pages': pages,
    }
    return render(request, 'movielist/index.html', context)


def info(request, title):
    context = {
        'title': title,
        'actors': movies[title]['actors'],
        'intro': movies[title]['intro'],
        'comments': movies[title]['comments'],
    }
    return render(request, 'movielist/infopage.html', context)


def actorinfo(request, name):
    name = name
    intro = actors[name]['intro']
    acted = actors[name]['acted']
    cooperated = sorted(actors[name]['coop'].items(), key=lambda x: x[1])[:10]
    context = {
        'name': name,
        'intro': intro,
        'acted': acted,
        'cooperated': cooperated,
    }
    return render(request, 'movielist/actorinfopage.html', context)


def actorindex(request, page=1):
    pages = filter(lambda x: x > 0 and x < 241, list(range(page - 2, page + 3)))
    print(actor_names[(page - 1) * 20:page * 20])
    context = {
        'actor_names': actor_names[(page - 1) * 20:page * 20],
        'pages': pages,
    }
    return render(request, 'movielist/actorindex.html', context)


def searchpage(request):
    return render(request, 'movielist/searchpage.html')

last_string = ''
last_choice = ''
last_ans = set()


def search_bk(string, choice):
    sttime = time.time()

    global last_string
    global last_choice
    global last_ans

    if string == last_string and choice == last_choice:
        return 0, last_ans
    if string == '' and choice == '':
        return 0, last_ans

    pat = re.compile(r".*?" + string + ".*?")

    ans = set()
    if choice == "movie":
        for title in movie_titles:
            if pat.search(title):
                ans.add(title)

        for name in actor_names:
            if pat.search(name):
                for movie in actors[name]['acted']:
                    ans.add(movie)

        listans = list(ans)
        for i in range(0, len(ans)):
            listans[i] = [listans[i], '/static/movielist/' + listans[i] + '.png', '/' + listans[i]]

    elif choice == "actor":
        for name in actor_names:
            if pat.search(name):
                ans.add(name)
        for title in movie_titles:
            if pat.search(title):
                for actor in movies[title]['actors']:
                    ans.add(actor)
        listans = list(ans)
        for i in range(0, len(ans)):
            listans[i] = [listans[i], '/static/movielist/actor/' + listans[i] + '.png', '/actor/' + listans[i]]
    else:
        listans = []
        for title in movie_titles:
            for comment in movies[title]['comments']:
                if pat.search(comment):
                    listans.append([comment, '/static/movielist/' + title + '.png', '/' + title])

    costtime = time.time() - sttime
    last_ans = listans
    last_string = string
    last_choice = choice
    return costtime, listans


def search(request, page=1):
    if 'c' in request.GET and request.GET['c']:
        choice = request.GET['c']
    else:
        choice = ''

    if 'q' in request.GET:
        string = request.GET['q']
    else:
        string = ''

    costtime, ans = search_bk(string, choice)

    if choice == "movie":
        searchstring = "搜索电影" + string
    elif choice == "actor":
        searchstring = "搜索演员" + string
    else:
        searchstring = "搜索影评" + string

    totalpages = len(ans)//20 + 1
    pages = filter(lambda x: x > 0 and x <= totalpages, list(range(page - 2, page + 3)))
        
    context = {
        'searchstring': searchstring,
        'costtime': costtime,
        'ans': ans[(page - 1) * 20:page * 20],
        'totalpages': totalpages,
        'cnt': len(ans),
        'pages': pages,
    }
    return render(request, 'movielist/searchresult.html', context)
