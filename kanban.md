## todo

- [ ] don't allow dragging/resizing beyond image bounds 
- [ ] allow naming the cropped areas 
- [ ] store metadata in cropped images (?) 
  tried to store jpeg comment but Qt
  doesn't support that apparently 
- [ ] show Qt version/library location in about box Settings singleton for
  preferences

## ideas

see also notes.md

- [ ] user can select a "white" pixel; then use flood fill to find the rest of such
  areas, then use "curve fitting" technique to fit rectangles 
- [ ] incorporate scanning directly (SANE API)
- [ ] recognize a distinctive pattern on the scanner lid (stipple?) where photos
  are not, for boundary detection
- [ ] try YOLO
- [ ] try <http://dlib.net> (at least that is C++)
- [ ] scan photos with duplex scanner; save both sides to one HEIC file.  Use 
  <https://aur.archlinux.org/packages/qt6-heic-image-plugin>
- [ ] some other suitable format (does any JPEG-family format offer
  multiple-thumbnail support?)
- [ ] PDF perhaps
- [ ] develop Qt Quick UI for viewing 2-sided photo files (probably out of scope
  for this repo though)

## deferred

- [ ] while dragging bottom handles, control key to constrain to even angle (but
  that's not very useful) 
- [ ] nudge handle while mouse is held down over it (arrows) (but the magnified
  corner views are good enough for fine adjustment)

## done

- [x] next/previous navigation through a set of images (given on command line, or
  with a mask in cwd) and load relevant templates if that feature is selected
  in preferences 
- [x] loading template loads the "original" image too, if not already loaded
  (preference?) 
- [x] some actual image processing! 
- [x] command arg for image to load 
- [x] save crop positions to text or XML file 
- [x] zoom 
- [x] rotate selection 90 degrees 
- [x] delete rect 
- [x] pan 
- [x] drag to move 
- [x] make handles bigger 
- [x] zoom to fit entire image, width or selection 
