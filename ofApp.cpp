#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableDepthTest();
	ofSetLineWidth(2.5);

	int size = 90;
	for (int x = ofGetWidth() * -0.5 + size * 0.5; x <= ofGetWidth() * 0.5 - size * 0.5; x += size) {

		for (int y = ofGetHeight() * -0.5 + size * 0.5; y <= ofGetHeight() * 0.5 - size * 0.5; y += size) {

			this->location_list.push_back(glm::vec3(x, y, 360));

			int rotate_param = ofRandom(4);
			this->rotate_param_list.push_back(rotate_param);

			this->deg_start_list.push_back(0);
			this->deg_end_list.push_back(90);
			this->flag_list.push_back(false);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->location_list.size(); i++) {

		if (ofGetFrameNum() % 9 == 0) {

			if (ofRandom(100) < 10) { this->flag_list[i] = true; }
		}

		if (this->flag_list[i]) {

			if (this->deg_end_list[i] < 90) {

				this->deg_end_list[i] += 10;
			}
			else {

				this->deg_start_list[i] += 10;
				if (this->deg_start_list[i] >= 90) {

					this->deg_end_list[i] = 0;
					this->deg_start_list[i] = 0;
					this->rotate_param_list[i] = (this->rotate_param_list[i] + 1) % this->rotate_param_list.size();
				}
			}

			if (this->deg_start_list[i] == 0 && this->deg_end_list[i] == 90) { this->flag_list[i] = false; }
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofRotateY(ofGetFrameNum());
	//ofRotateX(ofGetFrameNum() * 1.2);

	int size = 90;
	int radius = size * 0.5;
	int width = 10;
	for (int i = 0; i < 6; i++) {

		if (i < 4) {
			ofRotateY(90);
		}
		else if (i < 5) {

			ofRotateX(90);
		}
		else if (i < 6) {

			ofRotateX(180);
		}

		for (int i = 0; i < this->location_list.size(); i++) {

			ofPushMatrix();
			ofTranslate(this->location_list[i].x, this->location_list[i].y);
			ofRotate(90 * this->rotate_param_list[i]);

			vector<glm::vec3> vertices_1, vertices_2;
			for (int deg = this->deg_start_list[i]; deg <= this->deg_end_list[i]; deg += 2) {

				vertices_1.push_back(glm::vec3(size * -0.5 + (radius + width) * cos(deg * DEG_TO_RAD), size * -0.5 + (radius + width) * sin(deg * DEG_TO_RAD), 360));
				vertices_2.push_back(glm::vec3(size * -0.5 + (radius - width) * cos(deg * DEG_TO_RAD), size * -0.5 + (radius - width) * sin(deg * DEG_TO_RAD), 360));
			}

			this->draw_arc(vertices_1, vertices_2);

			vertices_1.clear();
			vertices_2.clear();
			for (int deg = 180 + this->deg_start_list[i]; deg <= 180 + this->deg_end_list[i]; deg += 2) {

				vertices_1.push_back(glm::vec3(size * 0.5 + (radius + width) * cos(deg * DEG_TO_RAD), size * 0.5 + (radius + width) * sin(deg * DEG_TO_RAD), 360));
				vertices_2.push_back(glm::vec3(size * 0.5 + (radius - width) * cos(deg * DEG_TO_RAD), size * 0.5 + (radius - width) * sin(deg * DEG_TO_RAD), 360));
			}

			this->draw_arc(vertices_1, vertices_2);

			ofPopMatrix();
		}
	}

	ofSeedRandom(39);

	ofColor color;
	ofFill();
	for (int i = 0; i < 90; i++) {

		color.setHsb(ofRandom(255), 130, 255);
		ofSetColor(color);

		auto location = glm::vec3(
			ofMap(ofNoise(ofRandom(1000) + ofGetFrameNum() * 0.01), 0, 1, -450, 450),
			ofMap(ofNoise(ofRandom(1000) + ofGetFrameNum() * 0.01), 0, 1, -450, 450),
			ofMap(ofNoise(ofRandom(1000) + ofGetFrameNum() * 0.01), 0, 1, -450, 450));
		ofDrawSphere(location, 30);
	}

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::draw_arc(vector<glm::vec3> vertices_1, vector<glm::vec3> vertices_2) {

	ofFill();
	ofSetColor(0);
	std::reverse(vertices_2.begin(), vertices_2.end());

	ofBeginShape();
	ofVertices(vertices_1);
	ofVertices(vertices_2);
	ofEndShape(true);

	ofNoFill();
	ofSetColor(255);

	ofBeginShape();
	ofVertices(vertices_1);
	ofEndShape(false);

	ofBeginShape();
	ofVertices(vertices_2);
	ofEndShape(false);

	//ofDrawLine(vertices_1[0], vertices_2[vertices_2.size() - 1]);
	//ofDrawLine(vertices_1[vertices_1.size() - 1], vertices_2[0]);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}