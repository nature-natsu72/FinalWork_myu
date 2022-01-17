#include "ofApp.h"

int width, height;
int t;
//GitHubで公開するので超丁寧に書きます！頑張る！！
 

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    ofBackground(0, 63, 113);
    ofEnableDepthTest();
    ofSetCircleResolution(60);
    
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    // スポットライトを配置
    light.setSpotlight();
    // 照明の位置
    light.setPosition(0, 0, -1500);
    // 鏡面反射光の色：キラキラの部分
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    // 拡散反射光の色
    light.setDiffuseColor(ofFloatColor(0.5, 0.5, 1.0));
    // 環境反射光の色
    light.setAmbientColor(ofFloatColor(0.98, 0.87, 0, 0.5));
    
    // 海岸メッシュの各頂点の色を初期化
    for (int x = -1300; x <= 1300; x++) {
        for (int y = -100; y <= 2000; y++) {
            mesh.addColor(ofFloatColor(0, 0.3, 0.6));
        }
    }
    
//    それっぽい音
    wind.load("wind.mp3");
    wind.setLoop(true);
    wind.play();
    sea.load("sea.mp3");
    sea.setLoop(true);
    sea.setVolume(0.5);
    sea.play();
    
    
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
//    海の描写
    //    ofSeedRandom：乱数が常に同じになるように、値を渡して乱数発生を設定する（oF）
    ofSeedRandom(39);

    this->mesh.clear();

    //    auto：型推論（cpp）
    auto span = 10;
    //    glm（cppの数学系ライブラリ）の使い方はここを参照　https://openframeworks.cc/documentation/glm/
    //    波の滑らかさ
    auto seed = glm::vec3(ofRandom(2000), ofRandom(2000), ofRandom(2000));
    
    //    海の横幅=x, 海の奥行き=y
    //    -3000<=x<=3000, -100<=y<=3000で最終（今は高速レンダリング用）→重過ぎて動かないから、画角を変えてみる
    for (int x = -3000; x <= 3000; x += span) {
        for (int y = -200; y <= 2000; y += span) {
            
            vector<glm::vec3> vertices;
            vertices.push_back(this->rotate(seed, glm::vec3(x, y, 0)));
            vertices.push_back(this->rotate(seed, glm::vec3(x + span, y, 0)));
            vertices.push_back(this->rotate(seed, glm::vec3(x + span, y + span, 0)));
            vertices.push_back(this->rotate(seed, glm::vec3(x, y + span, 0)));
            for (auto& vertex : vertices) {
                //    波の穏やかさ
                //    ofMap(hoge, a, b, c, d)：[a,b]を[c, d]に写像する線形写像を使ってhogeを写像する
                vertex.z = ofMap(vertex.z, -8, 8, -10, 10);
            }
            
            if (t > 1) {
                mesh.addColor(ofFloatColor(0.15, 0.27, 0.47));
            }else if (t > 5){
                mesh.addColor(ofFloatColor(0.77, 0.32, 0.19));
            }else if (t > 40){
                mesh.addColor(ofFloatColor(0, 0.3, 0.6));
            }else if (t > 60){
                mesh.addColor(ofFloatColor(0, 0.43, 0.67));
            }else
                mesh.addColor(ofFloatColor(0, 0.3, 0.6));
            this->mesh.addVertices(vertices);
            this->mesh.addIndex(this->mesh.getNumVertices() - 1);
            this->mesh.addIndex(this->mesh.getNumVertices() - 2);
            this->mesh.addIndex(this->mesh.getNumVertices() - 3);
            this->mesh.addIndex(this->mesh.getNumVertices() - 1);
            this->mesh.addIndex(this->mesh.getNumVertices() - 3);
            this->mesh.addIndex(this->mesh.getNumVertices() - 4);
        }
    }
    
//    砂浜の描写
    //  現在の全ての頂点情報を消去(vertices->頂点)
    sand.clearVertices();
    // 砂浜メッシュの全ての頂点位置を更新、それを頂点情報として追加
    for (float i = -1500; i <= 1500; i++) {
        float y=5;
        for (float j = 400; j <= 1000; j++) {
            float x = i;
            float z = j;
            sand.addVertex(ofVec3f(x, y, z));
        }
    }
    
    t++;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // ライトを有効にする
//    light.enable();
    
    cam.setPosition(0, 100, 800);
//    バリアングルカメラ
    this->cam.begin();
    
//    海の描写
    ofRotateXDeg(270);

    ofSetLineWidth(1);

    ofTranslate(0, 0, 0.01);

    ofSetColor(0);
    this->mesh.drawWireframe();

    ofSetLineWidth(3);

    ofTranslate(0, 400, 0);
    ofRotateXDeg(90);
    
    
//    空の描写（マジで工夫しまくってるから褒めて欲しいな^-^）
    
    int count = 0;
    for (float i=180; i>90; i-=0.5) {
        ofFill();
        double deg = ofDegToRad(i);
        ofSetColor(0, 13, 100);
        if (t>1){
            ofSetColor(219-count*5, 61-count, 54+count*2);
        }
        if(t>10){
            ofSetColor(219-count*5+t*5, 61-count+t*5, 54+count*2+t*5);
        }
        if(t>40){
            ofSetColor(119-count*5+t*5, 61-count+t*5, 54+count*2+t*5);
        }
        ofSetLineWidth(10);
        ofDrawLine(glm::vec3(-3000, sin(deg)*1800, cos(deg)*1800), glm::vec3(3000, sin(deg)*1800, cos(deg)*1800));
        count++;
    }
    
//    太陽の描写（これは平面だけどいい感じだから許してほしい）
    ofFill();
    ofSetColor(255, 255, 102+t*5);
    ofDrawCircle(glm::vec3(0,t*10-200,-1500), 200-t*2);
    if (t<2) {
        ofSetColor(0, 63, 113);
        ofDrawRectangle(glm::vec3(-200,-200,-1499), 400, 200);
    }
    

//    砂浜の描写
    for (int i=0; i<1000; i++) {
        ofFill();
        ofSetColor(141-i*0.5, 131-i*0.5, 99-i*0.5);
        ofDrawLine(glm::vec3(-2000, 0, 400+i*2), glm::vec3(2000, 0, 400+i*2));
    }
    // 砂浜メッシュの各頂点の色
    for (int x = -1500; x <= 1500; x++) {
        for (int y = 400; y <= 800; y++) {
            sand.addColor(ofFloatColor(0.55, 0.51, 0.38));
        }
    }
    
    glPointSize(1.0);
    sand.drawVertices();
    
//    鳥居の描写（自作関数drawCylinder2Ptを使ってます！）
    ofFill();
    ofSetColor(200, 0, 0);
    drawCylinder2Pt(glm::vec3(-13,0,800), glm::vec3(-13,50,800), 2);
    drawCylinder2Pt(glm::vec3(13,0,800), glm::vec3(13,50,800), 2);
    drawCylinder2Pt(glm::vec3(-20,40,800), glm::vec3(20,40,800), 2);
    drawCylinder2Pt(glm::vec3(-25,50,800), glm::vec3(25,50,800), 2);
    ofSetColor(218, 165, 32);
    ofDrawRectangle(glm::vec3(-4, 40, 802), 8, 10);

    this->cam.end();

}

glm::vec3 ofApp::rotate(glm::vec3 seed, glm::vec3 location) {

    //    パーリンノイズはここで使ってるよ！！！！！！！
    float deg_x = ofMap(ofNoise(seed.x, location.x * 0.003, location.y * 0.003 + ofGetFrameNum() * 0.0085), 0, 1, -180, 180);
    float deg_y = ofMap(ofNoise(seed.y, location.x * 0.003, location.y * 0.003 + ofGetFrameNum() * 0.0085), 0, 1, -180, 180);
    float deg_z = ofMap(ofNoise(seed.z, location.x * 0.003, location.y * 0.003 + ofGetFrameNum() * 0.0085), 0, 1, -180, 180);

    auto rotation_x = glm::rotate(glm::mat4(), deg_x * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
    auto rotation_y = glm::rotate(glm::mat4(), deg_y * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
    auto rotation_z = glm::rotate(glm::mat4(), deg_z * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

    auto tmp = glm::vec3(5 * cos(0 * DEG_TO_RAD), 5 * sin(0 * DEG_TO_RAD), 0);
    tmp = glm::vec4(tmp, 0) * rotation_z * rotation_y * rotation_x;

    return location + tmp;
}

//      鳥居爆誕関数（3次元空間内の2点を指定して円柱を作る関数）
void ofApp::drawCylinder2Pt(ofVec3f p, ofVec3f q, float r)
{
        ofCylinderPrimitive cylinder;
        ofVec3f v = q - p;
        cylinder.setHeight(v.length());
        ofVec3f z(0.f, 1.f, 0.f);
        ofQuaternion quat;
        quat.makeRotate(z, v);

        cylinder.setRadius(r);
        cylinder.resetTransform();
        cylinder.rotate(quat);
        cylinder.setPosition((p + q)*0.5f);
        cylinder.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
