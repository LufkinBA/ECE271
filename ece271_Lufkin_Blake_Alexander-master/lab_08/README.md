ECE 271 - Microcomputer Architecture and Applications  
Lab Report  
Spring 2020


Name
-----------
```
Blake Lufkin
```

Summary of Lab Status
-------
- [ ] Pre-lab Assignment  
- [ ] Incomplete task  
    - [ ] Sub-task 1:   
    - [ ] Sub-task 2:   
    - [ ] Sub-task 3:   

What is your something cool?
-------
using the joypad to adjust brightness of the green LED. 
Post-lab Questions
-------
1) since ARR needs to be under 65535(16 bit value) the PSC needs to be high enough to cut the PWM frequency to below that
number. Thus setting the timer to upcounting, PSC=159 ->PWM f=50kHz, ARR=49999, CCR=25000

2) since the max value of a 16 bit register is 65535 then making the PSC that number would give us the lowest
possible PWM frequency of 122Hz. 
Suggestions and Comments
-------
* Report the typos, errors, or suggestions into the "typos" session on piazza site (https://piazza.com/maine/spring2017/ece271/home)
* I offer one dollar each new typo or error in the latest edition (printed in May, 2016).

Lab Credits
-------
* Did you received any help from someone other than Prof. Zhu and lab teaching assistants?
