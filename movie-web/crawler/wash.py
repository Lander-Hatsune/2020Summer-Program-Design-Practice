import json

movies = json.loads(open('./movies.json').read())
actors = json.loads(open('./actors.json').read())

for key in movies.keys():
    movie = movies[key]
    movie['intro'] = movie['intro'].replace('<br/>', ' ')
    movie['intro'] = movie['intro'].replace('<BR>', ' ')
    movie['intro'] = movie['intro'].replace('<P>', ' ')
    movie['intro'] = movie['intro'].replace('</P>', ' ')
    movie['intro'] = movie['intro'].replace('<br />', ' ')

    for i in range(0, len(movie['comments'])):
        movie['comments'][i] = movie['comments'][i].replace('<br/>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('<BR>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('<P>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('</P>', ' ')
        movie['comments'][i] = movie['comments'][i].replace('<br />', ' ')

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

with open("./movies-washed.json", 'w') as f:
    f.write(json.dumps(movies))


for key in actors.keys():
    actor = actors[key]
    actor['intro'] = actor['intro'].replace('<br/>', ' ')
    actor['intro'] = actor['intro'].replace('<BR>', ' ')
    actor['intro'] = actor['intro'].replace('<P>', ' ')
    actor['intro'] = actor['intro'].replace('</P>', ' ')
    actor['intro'] = actor['intro'].replace('<br />', ' ')

    actors[key] = actor

with open('./actors-washed.json', 'w') as f:
    f.write(json.dumps(actors))

print("DONE")
