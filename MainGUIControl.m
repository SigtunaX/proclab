#import "MainGUIControl.h"

@implementation MainGUIControl

- (void)awakeFromNib
{
	[self showTextureGen:nil];
}

- (IBAction)showTextureGen:(id)sender
{
	[ViewTextureLib removeFromSuperview];

	[ViewContainer addSubview:ViewTextureGen];
	[ViewTextureGen setFrame:[ViewContainer bounds]];
	[ViewContainer setNeedsDisplay: YES];
	[ViewTextureGen setNeedsDisplay: YES];

	NSRect frame = [mainWindow frame];

	// Tamany minim del frame del Generador de Textures
	int app_width = 900;
	int app_height = 770;

	if (frame.size.width<app_width)		frame.size.width = app_width;
	if (frame.size.height<app_height)	frame.size.height = app_height;

	[mainWindow setFrame:frame display:YES animate:YES];
}

- (IBAction)showTextureLib:(id)sender
{
	[ViewTextureGen removeFromSuperview];
	
	[ViewContainer addSubview:ViewTextureLib];
	[ViewTextureLib setFrame:[ViewContainer bounds]];
	[ViewContainer setNeedsDisplay: YES];
	[ViewTextureLib setNeedsDisplay: YES];
	
	NSRect frame = [mainWindow frame];
	
	// Tamany minim del frame del Generador de Libreries
	int app_width = 900;
	int app_height = 770;
	
	if (frame.size.width<app_width)		frame.size.width = app_width;
	if (frame.size.height<app_height)	frame.size.height = app_height;
	
	[mainWindow setFrame:frame display:YES animate:YES];
	
}

- (void) didAddSubview:(NSView *) view
{
	unsigned int driveCount = [[ViewContainer subviews] count];
	NSRect viewFrame = [view frame];
	viewFrame.origin.y = (viewFrame.size.height * (driveCount - 1));
	[view setFrameOrigin: viewFrame.origin];
	NSRect myFrame = [ViewContainer frame];
	myFrame.size.width = viewFrame.size.width;
	myFrame.size.height = (viewFrame.size.height * driveCount);
	[ViewContainer setFrame: myFrame];
	NSLog(@"view: %@, frame Texture: %@, frame Container: %@", view,NSStringFromRect([view frame]), NSStringFromRect([ViewContainer frame]));
}

@end
