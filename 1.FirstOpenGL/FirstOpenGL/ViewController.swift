//
//  ViewController.swift
//  FirstOpenGL
//
//  Created by Stan on 2022/12/2.
//

import UIKit
import GLKit
import AVFoundation

class ViewController: GLKViewController {
    
    var glkView:GLKView!
    var content:EAGLContext!
    var myEffect:GLKBaseEffect!

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        
        
        
        
        glkView = view as? GLKView
        glkView.drawableColorFormat =  .RGBA8888;
        glkView.drawableDepthFormat = GLKViewDrawableDepthFormat.format24
        EAGLContext.setCurrent(content)
        glEnable(GLenum(GL_DEPTH_TEST))
        glClearColor(1, 1, 1, 1)
        
        content = EAGLContext(api: EAGLRenderingAPI.openGLES2);
        if (content == nil){
            print("Failed to load content!");
        }
        
        EAGLContext.setCurrent(content)
        glEnable(GLenum(GL_DEPTH_TEST))
        glkView.context = content
        
        
    }


}

