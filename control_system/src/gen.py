from enum import Enum
from itertools import permutations

class Color(Enum):
    Green = 0x4
    Yellow = 0x2
    Red = 0x1
    Empty = 0x0 
    YellowBlink = 0x8

invalidValue = Color.Green.value  << 8 | Color.Green.value

colors = [
    Color.Empty.value,
    Color.Red.value,
    Color.Yellow.value,
    Color.Green.value,
    Color.Red.value | Color.Yellow.value,
    Color.Red.value | Color.Green.value,                        # сомнительная комбинация (как и некоторые другие), но не противоречит условиям задания
    Color.Yellow.value | Color.Green.value,
    Color.Green.value | Color.Yellow.value | Color.Red.value,
    # + blink   
    Color.YellowBlink.value | Color.Empty.value,
    Color.YellowBlink.value | Color.Red.value,
    Color.YellowBlink.value | Color.Yellow.value,
    Color.YellowBlink.value | Color.Green.value,
    Color.YellowBlink.value | Color.Red.value | Color.Yellow.value,
    Color.YellowBlink.value | Color.Red.value | Color.Green.value,
    Color.YellowBlink.value | Color.Yellow.value | Color.Green.value,
    Color.YellowBlink.value | Color.Green.value | Color.Yellow.value | Color.Red.value,
]


for x in colors:
    for y in colors:
        z = x << 8 | y
        if (not (z & invalidValue == invalidValue)):
            print('message.value ==',hex(z),',')