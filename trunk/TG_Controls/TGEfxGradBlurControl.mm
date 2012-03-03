#import "TGEfxGradBlurControl.h"
#import "../TGGUIControl.h"

@implementation TGEfxGradBlurControl

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
	return [TGEfxGradBlurPanel isVisible];
}

- (void) showCtrl
{
	[TGEfxGradBlurPanel makeKeyAndOrderFront:nil];
}

- (void) hideCtrl
{
	[TGEfxGradBlurPanel orderOut:nil];
}

- (IBAction)redraw:(id)sender
{
    [self getValues];

	[(TGGUIControl*)_parent GetEfxGradBlur:t_data];	// Send the data to the parent
	[(TGGUIControl*)_parent renderTemp:nil];		// Redraw
}


- (void) getValues
{
	t_data.steps = (unsigned char)[TGPSteps floatValue];
	t_data.stepX = (unsigned short)[TGPStepX floatValue];
	t_data.stepY = (unsigned short)[TGPStepY floatValue];
}


@end
