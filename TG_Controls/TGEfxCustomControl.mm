#import "TGEfxCustomControl.h"
#import "../TGGUIControl.h"

@implementation TGEfxCustomControl

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
	return [TGEfxCustomPanel isVisible];
}

- (void) showCtrl
{
	[TGEfxCustomPanel makeKeyAndOrderFront:nil];
}

- (void) hideCtrl
{
	[TGEfxCustomPanel orderOut:nil];
}

- (IBAction)redraw:(id)sender
{
	int value;
	value = [val1 intValue];
	[val1 setStringValue:[NSString stringWithFormat:@"%d",value]];
	value = [val2 intValue];
	[val2 setStringValue:[NSString stringWithFormat:@"%d",value]];
	value = [val3 intValue];
	[val3 setStringValue:[NSString stringWithFormat:@"%d",value]];
	value = [val4 intValue];
	[val4 setStringValue:[NSString stringWithFormat:@"%d",value]];
	value = [val5 intValue];
	[val5 setStringValue:[NSString stringWithFormat:@"%d",value]];
	value = [val6 intValue];
	[val6 setStringValue:[NSString stringWithFormat:@"%d",value]];
	value = [val7 intValue];
	[val7 setStringValue:[NSString stringWithFormat:@"%d",value]];
	value = [val8 intValue];
	[val8 setStringValue:[NSString stringWithFormat:@"%d",value]];
	value = [val9 intValue];
	[val9 setStringValue:[NSString stringWithFormat:@"%d",value]];
	
	t_data.filter[0] = (char)[val1 intValue];
	t_data.filter[1] = (char)[val2 intValue];
	t_data.filter[2] = (char)[val3 intValue];
	t_data.filter[3] = (char)[val4 intValue];
	t_data.filter[4] = (char)[val5 intValue];
	t_data.filter[5] = (char)[val6 intValue];
	t_data.filter[6] = (char)[val7 intValue];
	t_data.filter[7] = (char)[val8 intValue];
	t_data.filter[8] = (char)[val9 intValue];
	
	
	t_data.cfactor = (char) [factor intValue];
	t_data.bias = (char) [bias intValue];
	
	
	[(TGGUIControl*)_parent GetEfxCustom:t_data];	// Send the data to the parent
	[(TGGUIControl*)_parent renderTemp:nil];		// Redraw
}


//- (IBAction) getNumber:(NSTextField)sender;
- (float)getNumber:(NSString*)value
{
	return [value floatValue];
}


@end
