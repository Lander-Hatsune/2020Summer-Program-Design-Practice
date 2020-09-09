import requests
import re
import time
import os
from PIL import Image
from io import BytesIO
import json

headers = {"Accept": '*/*',
           "Accept-Encoding": 'gzip, deflate',
           "Accept-Language": 'zh-CN,zh;q=0.9,en;q=0.8',
           "Connection": 'keep-alive',
           "Host": 'service.channel.mtime.com',"User-Agent": 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36'}

root_url0 = "http://service.channel.mtime.com/service/search.mcs?Ajax_CallBack=true&Ajax_CallBackType=Mtime.Channel.Pages.SearchService&Ajax_CallBackMethod=SearchMovieByCategory&Ajax_CrossDomain=1&Ajax_RequestUrl=http%3A%2F%2Fmovie.mtime.com%2Fmovie%2Fsearch%2Fsection%2F%3Frating%3D8_10%23pageIndex%3D3%26nation%3D138&t=2020971328564354&Ajax_CallBackArgument0=&Ajax_CallBackArgument1=0&Ajax_CallBackArgument2=138&Ajax_CallBackArgument3=0&Ajax_CallBackArgument4=0&Ajax_CallBackArgument5=0&Ajax_CallBackArgument6=0&Ajax_CallBackArgument7=0&Ajax_CallBackArgument8=&Ajax_CallBackArgument9=0&Ajax_CallBackArgument10=0&Ajax_CallBackArgument11=0&Ajax_CallBackArgument12=0&Ajax_CallBackArgument13=0&Ajax_CallBackArgument14=1&Ajax_CallBackArgument15=0&Ajax_CallBackArgument16=1&Ajax_CallBackArgument17=4&Ajax_CallBackArgument18="
root_url1 = "&Ajax_CallBackArgument19=0"

actors_url_suf = "fullcredits.html"
actors_intro_url_suf = "details.html"

movie_intro_url_suf = "plots.html"



# title & url
movie_urls_pattern = re.compile('<h3 class[\d|\D]*?href=\\\\"([\d|\D]*?)\\\\">([\d|\D]*?)</a>')
movie_intro_pattern = re.compile('<span class="first_letter">(.)</span>([\d|\D]*?)</p>')
movie_img_url_pattern = re.compile('<div class="db_cover __r_c_"[\d|\D]*?<img src="([\d|\D]*?)" alt')
movie_comment_pattern = re.compile('mod_short">[\d|\D]*?<h3>([\d|\D]*?)</h3>')

# name & url
actor_url_pattern = re.compile('<div class="actor_tit">.*?<h3><a href="([\d|\D]*?)".*?>([\d|\D]*?)</a>')
actor_intro_pattern = re.compile('<div class="per_biography" id="lblPartGraphy">[\d|\D]*?(<p></p>\w*)*<p>([\d|\D]*?)</p>')
actor_img_pattern = re.compile('<div class="per_cover __r_c_"[\d|\D]*?<img src="([\d|\D]*?)" alt')


cnt = -1
actor_info = {}
movie_info = {}

with open('./movies.json', 'r') as f:
    movie_info = json.loads(f.read())

origin_size = len(movie_info)
print(origin_size)


with open('./actors.json', 'r') as f:
    actor_info = json.loads(f.read())

#print(actor_info)
#print(movie_info)

i = 0


for pages in range(origin_size // 20 + 1, 60):
    page_url = root_url0 + str(pages) + root_url1
    # print('url: ', url)
    page_ans = requests.get(page_url, headers).text
    time.sleep(0.7)
    try:
        movielist = movie_urls_pattern.findall(page_ans, re.I | re.S)
        print('page: ', pages)
    except():
        print('error reading page', pages)
        continue
    i = (pages - 1) * 20

    for item in movielist:
        i += 1

        print('movie: ', i)
        movie_url = item[0]
        movie_title = item[1]

        if movie_info.get(movie_title):
            continue
        
        print(movie_title)
        this_movie = {}

        # actors
        all_actors_ans = requests.get(movie_url + actors_url_suf, headers).text
        time.sleep(0.7)

        with open('./movies-actors-html/' + movie_title + '.html', 'w') as f:
            f.write(all_actors_ans)

        actors = actor_url_pattern.findall(all_actors_ans, re.S | re.I)
        if not actors:
            print('actors error', movie_title)
            
        if len(actors) > 12:
            actors = actors[0:12]
        movie_actors_names = [actor[1] for actor in actors]
        this_movie['actors'] = movie_actors_names

        for actor in actors:
            actor_url = actor[0]
            actor_name = actor[1]
            print(actor_name)
            if actor_info.get(actor_name):
                if movie_title not in actor_info[actor_name]["acted"]:
                    actor_info[actor_name]["acted"].append(movie_title)
            else:
                this_actor = {}
                actor_ans = requests.get(actor_url, headers).text
                time.sleep(0.7)

                with open('./actors-html/' + actor_name + '.html', 'w') as f:
                    f.write(actor_ans)
                
                if len(actor_ans) < 200:
                    continue
                
                # actor intro
                actor_intro_ans = requests.get(actor_url + actors_intro_url_suf, headers).text
                time.sleep(0.7)

                with open('./actors-intro-html/' + actor_name + '.html', 'w') as f:
                    f.write(actor_intro_ans)
                
                actor_intro_match = actor_intro_pattern.search(actor_intro_ans)
                if actor_intro_match:
                    actor_intro = actor_intro_match.group(2)
                    if len(actor_intro) < 10:
                        actor_intro = "暂无"
                else:
                    actor_intro = "暂无"
                this_actor["intro"] = actor_intro
                print(actor_intro)

                # img
                try:
                    actor_img_url_match = actor_img_pattern.search(actor_ans, re.S | re.I)
                    actor_img_url = actor_img_url_match.group(1)
                    actor_img = requests.get(actor_img_url, headers)
                    time.sleep(0.7)
                    try:
                        Image.open(BytesIO(actor_img.content)).save("./actors-img/" + actor_name + ".png")
                    except(OSError):
                        print("actor image error: ", movie_title, actor_name)

                except(AttributeError):
                    print('getting actor img error', movie_title, actor_name)


                # acted
                this_actor['acted'] = [movie_title]

                actor_info[actor_name] = this_actor

        # movie intro
        movie_intro_ans = requests.get(movie_url + movie_intro_url_suf, headers).text
        
        with open('./movies-intro-html/' + movie_title + '.html', 'w') as f:
            f.write(movie_intro_ans)
                    
        match = movie_intro_pattern.search(movie_intro_ans, re.S | re.I)
        if match:
            movie_intro = match.group(1) + match.group(2)
        else:
            movie_intro = "暂无"
        this_movie['intro'] = movie_intro
        print(movie_intro)

        # movie img
        movie_ans = requests.get(movie_url, headers).text
        with open("./movies-html/" + movie_title + ".html", "w") as f:
            f.write(movie_ans)
        movie_img_url_match = movie_img_url_pattern.search(movie_ans, re.S | re.I)
        if movie_img_url_match:
            movie_img_url = movie_img_url_match.group(1)
        else:
            print("movie img error", movie_title)
            continue
        movie_img = requests.get(movie_img_url, headers)
        time.sleep(0.7)
        try:
            Image.open(BytesIO(movie_img.content)).save("./movies-img/" + movie_title + ".png")
        except(OSError):
            print("movie image error: ", movie_title)
            continue


        # movie comment
        comments = movie_comment_pattern.findall(movie_ans, re.S | re.I)
        this_movie["comments"] = comments
        print(comments)
            
        # end
        movie_info[movie_title] = this_movie

        if i > origin_size:
            with open('actors.json', 'w', encoding = 'utf-8') as f:
                f.write(json.dumps(actor_info, ensure_ascii = False))

            with open('movies.json', 'w', encoding = 'utf-8') as f:
                f.write(json.dumps(movie_info, ensure_ascii = False))


with open('actors.json', 'w', encoding = 'utf-8') as f:
    f.write(json.dumps(actor_info, ensure_ascii = False))

with open('movies.json', 'w', encoding = 'utf-8') as f:
    f.write(json.dumps(movie_info, ensure_ascii = False))


    
print('DONE')
