# TODO List
[ ] learn how to draw a grid in SDL
    [x] figure out how to calculate the x/y cordinates 
        of each cell
    [x] figure out how to fill whole window
    [ ] figure out how to either fit perfect or make 
        grid span forever
[x] make grid clickable
[ ] refactor to make it easier to draw gui elements
    - Can I attach all gui elements to the app?
    - On mouse click, I can see which element was selected
      based on x,y cordinates.
    - Get the element and call its clicked function
[ ] play/pause buttons
[ ] generation counter
[ ] Slow down frames

# GUI System
[ ] make elements clickable

How am I going to do that?
- Everything is essentially a rectangle
- A stage should have a collection of gui elements
- each gui element should have some rendering logic associated with it
- I can loop through each gui element and call it's draw function
- Each element should be able to tell if it was clicked
- I need to figure out how to render the text properly
