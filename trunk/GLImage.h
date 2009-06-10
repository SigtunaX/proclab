//
//  GLImage.h
//  GeneratorLab

#import <Cocoa/Cocoa.h>

@interface GLImage : NSObject {

	NSButton		*	_visible;
    NSImage			*	_defaultThumbnail;
    NSComboBoxCell	*	_operation;
	NSString		*	_title;

}

#pragma mark Accessors

- (NSButton*)visible;
- (void)setVisible:(NSButton*)aValue;

- (NSImage*)defaultThumbnail;
- (void)setDefaultThumbnail:(NSImage*)aValue;

- (NSComboBoxCell*)operation;
- (void)setOperation:(NSComboBoxCell*)aValue;

- (NSString*)title;
- (void)setTitle:(NSString*)aValue;

@end
