#import <Cocoa/Cocoa.h>
#import "c_structs.h"
#import "../textgenlib/main.h"

@interface tableviewLYR : NSObject {
	NSMutableArray				*records;
	IBOutlet NSTableView		*TableView;
	IBOutlet id *_parent;
}

-(int)selectedRow;
-(void)selectRow:(int)row;
-(int)numberOfRows;
-(void)addLayer:(NSString*)enabled image:(NSImage*)thumb operation:(int)oper_ind properties:(NSString*)props;
-(void)deleteLayerAtPos: (int)pos;
-(void)deleteallLayers;

@end
