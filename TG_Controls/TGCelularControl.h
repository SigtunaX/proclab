#import "c_structs.h"
#import "../textgenlib/main.h"

@interface TGCelularControl : NSObject {
    IBOutlet NSColorWell *TGCColor1;
    IBOutlet NSColorWell *TGCColor2;
	IBOutlet NSComboBox *TGCType;
    IBOutlet NSPanel *TGCelularPanel;
    IBOutlet NSObject *_parent;
@public
	c_gfx	*gfx;
	T_CELULAR t_data; // Texture data
}
- (void) showCtrl;
- (void) hideCtrl;
- (BOOL) isvisibleCtrl;

- (IBAction) redraw:(id)sender;
- (NSColor *) getColor1;
- (NSColor *) getColor2;
- (void) getType;

@end
