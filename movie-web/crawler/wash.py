import json
import re

movies = json.loads(open('./movies.json').read())
actors = json.loads(open('./actors.json').read())

errpat1 = re.compile(r"<a href.*?>")

for key in movies.keys():
    movie = movies[key]
    movie['intro'] = movie['intro'].replace('<br/>', ' ')
    movie['intro'] = movie['intro'].replace('<BR>', ' ')
    movie['intro'] = movie['intro'].replace('<P>', ' ')
    movie['intro'] = movie['intro'].replace('</P>', ' ')
    movie['intro'] = movie['intro'].replace('<br />', ' ')
    movie['intro'] = movie['intro'].replace('<p>', ' ')
    movie['intro'] = movie['intro'].replace('</a>', ' ')
    movie['intro'] = errpat1.sub(string=movie['intro'], repl='')

    for i in range(0, len(movie['comments'])):
        movie['comments'][i] = movie['comments'][i].replace('<br/>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('<BR>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('<P>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('</P>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('<br />', ' ')

        movie['comments'][i] = movie['comments'][i].replace('<p>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('</a>', ' ')
        movie['comments'][i] = errpat1.sub(string=movie['comments'][i], repl='')

    movies[key] = movie
    for actor_name in movie['actors']:
        if not actors.get(actor_name):
            movie['actors'].remove(actor_name)
            continue
        actor = actors[actor_name]
        if not actor.get('coop'):
            actor['coop'] = {}
        for coop_actor_name in movie['actors']:
            if coop_actor_name == actor_name:
                continue
            if not actor['coop'].get(coop_actor_name):
                actor['coop'][coop_actor_name] = 0
            actor['coop'][coop_actor_name] += 1
        actors[actor_name] = actor

with open("./movies-washed.json", 'w') as f:
    f.write(json.dumps(movies))


for key in actors.keys():
    actor = actors[key]
    actor['intro'] = actor['intro'].replace('<br/>', ' ')
    actor['intro'] = actor['intro'].replace('<BR>', ' ')
    actor['intro'] = actor['intro'].replace('<P>', ' ')
    actor['intro'] = actor['intro'].replace('</P>', ' ')
    actor['intro'] = actor['intro'].replace('<br />', ' ')

    actor['intro'] = actor['intro'].replace('<p>', ' ')
    actor['intro'] = actor['intro'].replace('</a>', ' ')
    actor['intro'] = errpat1.sub(string=actor['intro'], repl='')

    actors[key] = actor

with open('./actors-washed.json', 'w') as f:
    f.write(json.dumps(actors))

print("DONE")
