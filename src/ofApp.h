#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void drawCylinder2Pt(ofVec3f p, ofVec3f q, float r);

//        ofBoxPrimitive box; // 立方体プリミティブ
//        ofSpherePrimitive sphere; // 球プリミティブ
//        ofEasyCam cam; // カメラ
    
        glm::vec3 rotate(glm::vec3 seed, glm::vec3 location);

        ofEasyCam cam;
        ofMesh mesh;
        ofMesh sand;
        ofCylinderPrimitive torii1, torii2, torii3, torii4;
        ofLight light;
    
        ofSoundPlayer sea, wind;

};
