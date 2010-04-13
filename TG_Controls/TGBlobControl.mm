#import "TGBlobControl.h"
#import "../TGGUIControl.h"

@implementation TGBlobControl

- (id)init
{
	if (self = [super init])
	{
	}
	return self;
}

- (void)dealloc
{
	[super dealloc];
}

- (void)awakeFromNib
{
}

- (BOOL) isvisibleCtrl
{
	return [TGBlobPanel isVisible];
}

- (void) showCtrl
{
	[TGBlobPanel makeKeyAndOrderFront:nil];
}

- (void) hideCtrl
{
	[TGBlobPanel orderOut:nil];
}

- (IBAction)redraw:(id)sender
{
	[self getValues];
	[(TGGUIControl*)_parent GetBlobData:t_data];	// Send the data to the parent
	[(TGGUIControl*)_parent renderTemp:nil];		// Redraw
}

- (void) getValues
{

	NSColor *myColor = [TGBlColor color];
	t_data.c.R = 255*[myColor redComponent];
	t_data.c.G = 255*[myColor greenComponent];
	t_data.c.B = 255*[myColor blueComponent];
	
	t_data.inv = (unsigned char)[TGBlInv state];
	t_data.mad = (unsigned char)[TGBlMad state];
	t_data.madvalor = (unsigned char)[TGBlMadValue  floatValue];
	t_data.zoom = (unsigned char)[TGBlzoom  floatValue];
}

@end
