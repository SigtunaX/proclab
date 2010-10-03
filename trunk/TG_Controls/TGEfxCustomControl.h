#import "c_structs.h"
#import "../textgenlib/main.h"

@interface TGEfxCustomControl : NSObject {
	IBOutlet NSTextField *val1;
	IBOutlet NSTextField *val2;
	IBOutlet NSTextField *val3;
	IBOutlet NSTextField *val4;
	IBOutlet NSTextField *val5;
	IBOutlet NSTextField *val6;
	IBOutlet NSTextField *val7;
	IBOutlet NSTextField *val8;
	IBOutlet NSTextField *val9;
	IBOutlet NSSlider *bias;
	IBOutlet NSSlider *factor;
    IBOutlet NSPanel *TGEfxCustomPanel;
    IBOutlet NSObject *_parent;
@public
	c_gfx	*gfx;
	T_EFF_CUSTOM t_data; // Effect data
}

- (void) showCtrl;
- (void) hideCtrl;
- (BOOL) isvisibleCtrl;
- (float) getNumber:(NSString*)value;

- (IBAction) redraw:(id)sender;

@end
