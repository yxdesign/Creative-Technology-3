// Copyright (c) 2019 ml5
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/* ===
ml5 Example
PoseNet example using p5.js
=== */

let img;
let cnv;
let webcam;
let poseNet;
let poses = [];
noseposition = 640;

function preload (){
  img = loadImage ('windowSwap.jpg');
}

function setup() {
  cnv = createCanvas(640, 358.4);
  
  //stream the webcam footage to the canvas
  webcam = createCapture(VIDEO);
  webcam.size(width, height);
  image (img, 0, 0, 640, 358.4);
  
  // Create a new poseNet method with a single detection
  poseNet = ml5.poseNet(webcam, modelReady);
  // This sets up an event that fills the global variable "poses"
  // with an array every time new poses are detected
  poseNet.on("pose", function (results) {
    poses = results;
    
    try{
      if (poses[0].pose.nose.confidence >= 0.01) {
        nosepositionx = poses[0].pose.nose.x;
        nosepositiony = poses[0].pose.nose.y;
        //print the x and y position of the nose
        console.log(nosepositionx,nosepositiony);
        
        let xmax = nosepositionx + 30;
        let xmin = nosepositionx - 30;
        let ymax = nosepositiony + 10;
        let ymin = nosepositiony - 10;
        
        //distort the image based on the location of the nose
        for(let col = xmin; col <xmax; col+=2){
          for (let row = ymin; row <ymax; row+=2){
            let xPos = col;
            let yPos = row;
            let c = img.get(xPos, yPos);
            push();
            translate (xPos, yPos);
            rotate (radians(random(45)));
            noFill();
            strokeWeight(random (3));
            stroke(color(c));
            curve (xPos, yPos, sin(xPos) * random (60), cos(xPos) * sin(xPos) * random (90), random (80), 10, cos (yPos) * sin (yPos) * random(140), cos(xPos) * sin(xPos) * 50);
            pop ();
          }
        }
      }
    } catch (err) {
      console.log(err);
    }
  });
}

function modelReady() {
  select("#status").html("Model Loaded");
}