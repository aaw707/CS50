# start the game
def main():
    level = 0 # after passing level 1, level = 1
    max_level = 7 # 7 levels in total. after passing level 7, level = 7
    while level < max_level:
        start_level(level)
    print('You have passed all the levels!')

# start a specific level based on input
def start_level(level):
    if level == 0:
        level1()
    elif level == 1:
        level2()
    elif level == 2:
        level3()
    elif level == 3:
        level4()
    elif level == 4:
        level5()
    elif level == 5:
        level6()
    elif level == 6:
        level7()
    
# levels
def level1():
    draw a map
    add a sprite
    if touch edge: end
    if reach endpoint: level + 1, then end

def level2():
    draw a map
    add 2 sprites
    let sprite 2 follow sprite 1, with a delay
    if touch edge: end
    if reach endpoint: level + 1, then end
    
def level3():
    kinda like level 2

def level4():
    draw a map
    add 4 sprites
    let sprite 2 and 3 follow sprite 1, with a delay
    let sprite 4 patrol around
    if touch edge: end
    if touch sprite 5: end
    if reach endpoing: level + 1, then end

def level5, level6, level7:
    kinda like level 4, with more patrollers

...

level1: harry
level2: ron following harry
level3: ron and hermione following harry
level4: ron and hermione following harry avoiding draco
level5: ron and hermione following harry avoiding draco and filtch
level6: ron and hermione following harry avoiding draco and filtch and snape
level7: ron and hermione following harry avoiding draco and filtch and snape and mcgonagall

def follow(person):
    let ron and hermione follow harry


##### turns out scratch works different than I thought...
##### I need to program by sprites
##### lesson: learn your tool before make a plan of using it...

