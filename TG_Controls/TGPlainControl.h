//
//  TGPlainControl.h
//  proclab
//
//  Created by xphere on 19/04/08.
//  Copyright 2008 Zona Neutra / Spöntz. All rights reserved.
//

#import "c_structs.h"
#import "../textgenlib/main.h"

@interface TGPlainControl : NSObject {
	IBOutlet NSPanel *TGPlainPanel;
	IBOutlet NSColorWell *TGPColor1;
	IBOutlet NSObject *_parent;
@public
	c_gfx	*gfx;
	T_PLAIN t_data; // Texture data
}
- (void) showCtrl;
- (void) hideCtrl;
- (BOOL) isvisibleCtrl;

- (IBAction) redraw:(id)sender;
- (NSColor *) getColor1;
@end
