//: Playground - noun: a place where people can play

import UIKit
import WatchKit

var str = "Hello, playground"

// Create a graphics context
let size = CGSizeMake(103, 100)
UIGraphicsBeginImageContext(size)
let context = UIGraphicsGetCurrentContext()

// Setup for the path appearance
UIColor.greenColor().setStroke()
UIColor.greenColor().setFill()

// Draw an oval
let total = 190
var bar = 100
var proportion = Double(bar) / Double(total) * 100
var fill = 100 - proportion
var width = proportion
let rect = CGRectMake(CGFloat(width + 2), 2, CGFloat(fill), 20)
let containRect = CGRectMake(2, 2, 100, 20)
let path = UIBezierPath(rect: rect)
let containPath = UIBezierPath(rect: containRect)
path.lineWidth = 1.0
UIColor.redColor().setFill()
//containPath.fill()
UIColor.greenColor().setFill()
path.fill()
path.stroke()
containPath.lineWidth = 1.0
containPath.stroke()


// Convert to UIImage
let cgimage = CGBitmapContextCreateImage(context);
let uiimage = UIImage(CGImage: cgimage!)

// End the graphics context
UIGraphicsEndImageContext()


