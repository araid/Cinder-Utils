Texture Mask
============

This is a utility class that solves a common problem: masking two images of different size.

There are two ways of approaching this, demonstrated in two samples:

1. Calling the static method `MaskedTexture::drawMaskedTexture(imageTex, maskTex, imageRect, maskRect)`.      
This will draw a rectangle whose vertices have 2 sets of coordinates, one for the main image and one for the mask, according to the supplied rectangles. It will calculate those coordinates every time it's called.     
Use it for simple scenes or when the position of the mask in relation to the image is going to change often.
![Preview1](https://raw.githubusercontent.com/araid/Cinder-Utils/master/TextureMask/preview1.png)

2. Creating and saving a VboMesh with `MaskedTexture::createMaskedTextureMesh(mImageRect, mMaskRect)` and then drawing it with `        MaskedTexture::drawMaskedTextureMesh(imageTexture, maskTexture, mesh)`.      
This has the advantage that texture coordinates are only calculated once and saved in a VboMesh that you can draw at almost no cost.      
Use it for scenes where the mask is not going to change.
![Preview2](https://raw.githubusercontent.com/araid/Cinder-Utils/master/TextureMask/preview2.png)

The shaders used in these functions are very basic and you might prefer to incorporate them to your own. In some applications you might even only want to create the Vbo Mesh and then handle the drawing yourself, in order to avoid redundant binding and unbinding of shaders and textures.

The class also supports ARB textures with an optional boolean at the end. That can be very handy when getting the texture from a Quicktime video, for example.
