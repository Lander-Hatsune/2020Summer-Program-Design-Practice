import os
import json
import re

actors = json.loads(open('actors-washed-washed.json').read())
pat = re.compile(r"(.*?)&#\d{3};(.*)")

names = actors.keys()
new_actors = {}
for name in names:
    if pat.findall(name):
        print(pat.findall(name))
        new_name = '-'.join(pat.findall(name)[0])
        new_actors[new_name] = actors[name]
        try:
            os.rename('./actors-img/' + name + '.png',
                      './actors-img/' + new_name + '.png')
        except(FileNotFoundError):
            print(FileNotFoundError)
    else:
        new_actors[name] = actors[name]    

with open('actors-washed-washed-washed.json', 'w') as f:
    f.write(json.dumps(new_actors))
