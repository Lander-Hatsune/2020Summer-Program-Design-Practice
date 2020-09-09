from django.shortcuts import render
from django.http import HttpResponse
from django.shortcuts import render
from .models import Movie

import json

# Create your views here.

movies = json.loads(open('./data/jsons/movies.json').read())
actors = json.loads(open('./data/jsons/actors.json').read())

movie_titles = list(movies.keys())

for i in movie_titles:
    m = Movie(movie_title=i,
              movie_img_url='movies_list/' + i + '.png')
    m.save()

def index(request, page=1):
    context = {
        'movies': Movie.objects.order_by('id')[20 * (page - 1):20 * (page)]
    }
    return render(request, 'movies_list/index.html', context)

def info(request, title):
    print(title)
    print(movies[title])
    return HttpResponse(str(movies[title]))
