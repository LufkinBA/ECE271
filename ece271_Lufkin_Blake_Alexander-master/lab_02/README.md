ECE 271 - Microcomputer Architecture and Applications  
Lab Report  
Spring 2017     



Name
-----------
```
Blake Lufkin
```

Summary of Lab Status
-------
- [x] Pre-lab Assignment  
- [ ] Incomplete task  
    - [ ] Sub-task 1:   
    - [ ] Sub-task 2:   
    - [ ] Sub-task 3:   

What is your something cool?
-------
Displays scrolling message

Post-lab Questions
-------
1. 30 seg and 4 com lines. thus 34 pins.
2. Yes, the COM driver sends a constant signal which is sent through the segment lines. Each segment line controls 4 segments.
3. 176 max pixels. 16x32-bit registers thus, 512 bits
4. 96 pixels/segments
5. I initally thought that it would display whatever was still in the register before the displaying of the string but 
after testing I found that if you implement LCD_Clear before displaying the string, the segments will display as empty. 

Suggestions and Comments
-------
* Report the typos, errors, or suggestions into the "typos" session on piazza site (https://piazza.com/maine/spring2017/ece271/home)
* I offer one dollar each new typo or error in the latest edition (printed in May, 2016).

Lab Credits
-------
* Did you received any help from someone other than Prof. Zhu and lab teaching assistants?
