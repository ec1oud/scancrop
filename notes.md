# Tried:

~/src/x3f-tools/deps/src/opencv/samples/cpp has the examples built. 

example_cpp_intelligent_scissors is too slow on large images, and is just as
happy to trace people as the image borders.

example_cpp_grabcut is slow, works, but has a tendency to separate light parts
of background within the photo.  Also can leave out chunks.  Fitting a rotated
rectangle to the results might be ok if we don't care about capturing the photo
border, or perhaps fit to the exposed part of the photo first and then expand
the rect by a fixed amount.

example_cpp_watershed is practically useless: way too dependent on what you
draw first, doesn't always do any segmentation at all.

example_cpp_squares looks almost accurate enough on the first try. But
it generates a lot of redundant squares so far.


# Ideas:

question to gpt4all:

> I'm looking for a library to detect the boundaries of photos scanned from
> photo album pages.  I suspect there's a good way with OpenCV.  Perhaps edge
> detection first, then somehow finding combinations of the edges that fit
> rectangles.  But maybe there's a better way? 

> > There are several libraries available for image processing and analysis in
> > Python such as Pillow (Python Imaging Library), Scikit-Image or PyTorch Image
> > Module which can be used to detect boundaries of scanned photos from photo
> > album pages using edge detection followed by finding combinations of the edges
> > that fit rectangle shapes

