#import "apple_font.h"

void RenderFontApple (int x, int y, int size, int w, char *str, unsigned char *data)
{
	NSString  *text = [NSString stringWithCString:str];
	NSDictionary *textAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
		[NSFont userFixedPitchFontOfSize: size/3], NSFontAttributeName,
		[NSColor redColor],NSForegroundColorAttributeName,
		nil];
	
	NSImage *MyImage = [[NSImage alloc] initWithContentsOfFile:@"/Users/xphere/Desktop/Imagen 1.png"];
    //[MyImage setFlipped:YES]; // text likes to be drawn in flipped coords...
    [MyImage lockFocus];
    [text drawAtPoint:NSMakePoint(x,y) withAttributes:textAttributes];
    [MyImage unlockFocus];

	NSBitmapImageRep* bmp = [[NSBitmapImageRep alloc]initWithData: [MyImage TIFFRepresentation]];
	memcpy(data,[bmp bitmapData],256*256*3);
}
