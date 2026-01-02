#import <UIKit/UIKit.h>

UIImage* lynxiOSGetImageData(void* data, int width, int height, int dataLength)
{
    // Create a CGImage with the pixel data
    // If your OpenGL ES content is opaque, use kCGImageAlphaNoneSkipLast to ignore the alpha channel
    // otherwise, use kCGImageAlphaPremultipliedLast
    CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, data, dataLength, NULL);
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    CGImageRef iref = CGImageCreate(width, height, 8, 32, width * 4, colorspace, kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast,
                                    ref, NULL, true, kCGRenderingIntentDefault);
    
    // OpenGL ES measures data in PIXELS
    // Create a graphics context with the target size measured in POINTS
    NSInteger widthInPoints, heightInPoints;
    if (NULL != UIGraphicsBeginImageContextWithOptions) 
    {
        // On iOS 4 and later, use UIGraphicsBeginImageContextWithOptions to take the scale into consideration
        // Set the scale parameter to your OpenGL ES view's contentScaleFactor
        // so that you get a high-resolution snapshot when its value is greater than 1.0
        CGFloat scale = 1.0f;//eaglview.contentScaleFactor;
        widthInPoints = width / scale;
        heightInPoints = height / scale;
        UIGraphicsBeginImageContextWithOptions(CGSizeMake(widthInPoints, heightInPoints), NO, scale);
    }
    else 
    {
        // On iOS prior to 4, fall back to use UIGraphicsBeginImageContext
        widthInPoints = width;
        heightInPoints = height;
        UIGraphicsBeginImageContext(CGSizeMake(widthInPoints, heightInPoints));
    }
    
    CGContextRef cgcontext = UIGraphicsGetCurrentContext();
    
    // UIKit coordinate system is upside down to GL/Quartz coordinate system
    // Flip the CGImage by rendering it to the flipped bitmap context
    // The size of the destination area is measured in POINTS
    CGContextSetBlendMode(cgcontext, kCGBlendModeCopy);
    CGContextDrawImage(cgcontext, CGRectMake(0.0, 0.0, widthInPoints, heightInPoints), iref);
    
    // Retrieve the UIImage from the current context
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    
    UIGraphicsEndImageContext();
    
    CFRelease(ref);
    CFRelease(colorspace);
    CGImageRelease(iref);
    
    return image;
}

void lynxiOSReleaseImageData(UIImage* image)
{
}

void lynxiOSSaveImageDataToPhoto(void* data, int width, int height, int dataLength)
{
    // Retrieve the UIImage from the current context
    UIImage *image = lynxiOSGetImageData(data, width, height, dataLength);
    UIImageWriteToSavedPhotosAlbum(image, 0, NULL, NULL);
}

void lynxiOSGetImagePNGRepresentation(void** ppdata, int* len, void* data, int width, int height, int dataLength)
{
    // Retrieve the UIImage from the current context
    UIImage *image = lynxiOSGetImageData(data, width, height, dataLength);
    NSData* Data = UIImagePNGRepresentation(image);
    *ppdata = [Data bytes];
    *len = [Data length];
}

void lynxiOSGetImageJPGRepresentation(void** ppdata, int* len, void* data, int width, int height, int dataLength, float compress)
{
    // Retrieve the UIImage from the current context
    UIImage *image = lynxiOSGetImageData(data, width, height, dataLength);
    NSData* Data = UIImageJPEGRepresentation(image, compress);
    *ppdata = [Data bytes];
    *len = [Data length];
}




