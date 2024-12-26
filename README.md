# scancrop

A utility for scanning multiple photos at once on a flatbed scanner or with a
camera, and extracting them to individual files, cropped and rotated as
necessary. You can scan directly via SANE API or open image files, and draw
boxes around the images to crop out. If the box is a trapezoid, reverse
perspective transformation is done to stretch it back to the presumed
rectangular shape. 

Built originally with Qt 4. Now using Qt 6.

