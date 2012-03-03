#import "c_structs.h"
#import "../textgenlib/main.h"

@interface TGBlobControl : NSObject {
    IBOutlet NSColorWell *TGBlColor;
	IBOutlet NSButton *TGBlMad;
	IBOutlet NSButton *TGBlInv;
	IBOutlet NSSlider *TGBlzoom;
	IBOutlet NSSlider *TGBlMadValue;
	IBOutlet NSPanel *TGBlobPanel;
    IBOutlet NSObject *_parent;
@public
	c_gfx	*gfx;
	T_BLOB t_data; // Texture data
}

- (void) showCtrl;
- (void) hideCtrl;
- (BOOL) isvisibleCtrl;

- (IBAction) redraw:(id)sender;
- (void) getValues;

@end
