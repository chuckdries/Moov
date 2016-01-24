//
//  InterfaceController.swift
//  moov WatchKit Extension
//
//  Created by Chuck Dries on 1/23/16.
//  Copyright © 2016 Chuck Dries. All rights reserved.
//

import WatchKit
import Foundation


class InterfaceController: WKInterfaceController {

    @IBOutlet var image: WKInterfaceImage!
    let mildlyDarkGreenColor = UIColor(red: 142/255,green: 179/255,blue: 59/255, alpha: 1)
    let mildlyDarkRedColor = UIColor(red: 199/255,green:  86/255,blue:  70/255,alpha:  1)
    override func awakeWithContext(context: AnyObject?) {
        super.awakeWithContext(context)
        
        // Configure interface objects here.
    }

    override func willActivate() {
        // This method is called when watch view controller is about to be visible to user
        super.willActivate()
        draw()
    }

    override func didDeactivate() {
        // This method is called when watch view controller is no longer visible
        super.didDeactivate()
    }
    let total = 190
    var bar = 0
    let delta = 10
    @IBAction func minus() {
        if bar > delta {
            bar = bar - delta
        } else {
            bar = 0
        }
        draw()
    }
    @IBAction func plus() {
        if bar < (total - delta){
            bar = bar + delta
        } else {
            bar = total
        }

        draw()
    }
    @IBAction func zero() {
        bar = 0
        draw()
    }
    func draw() {
        
        // Create a graphics context
        let size = CGSizeMake(103, 100)
        UIGraphicsBeginImageContextWithOptions(size, false, 0)
        let context = UIGraphicsGetCurrentContext()
        
        // Setup for the path appearance
        mildlyDarkGreenColor.setStroke()

        
        // Draw an oval
        
        let proportion = Double(bar) / Double(total) * 100
        let fill = 100 - proportion
        let width = proportion
        let rect = CGRectMake(CGFloat(width + 2), 2, CGFloat(fill), 20)
        let containRect = CGRectMake(2, 2, 100, 20)
        let path = UIBezierPath(rect: rect)
        let containPath = UIBezierPath(roundedRect: containRect, cornerRadius: 3)
        path.lineWidth = 1.0
        mildlyDarkRedColor.setFill()
        containPath.fill()
        mildlyDarkGreenColor.setFill()
        path.fill()
//        path.stroke()
        containPath.lineWidth = 4.0
        containPath.stroke()
        
        
        // Convert to UIImage
        let cgimage = CGBitmapContextCreateImage(context);
        let uiimage = UIImage(CGImage: cgimage!)
        
        // End the graphics context
        UIGraphicsEndImageContext()
        
        image.setImage(uiimage)

    }


}
