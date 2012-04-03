#include "testApp.h"

#include "ofxValue.h"

//--------------------------------------------------------------
void testApp::setup()
{
	{
		// typecheck
		
		ofxValue v = 10;
		
		assert(!v.is<string>());
		assert(!v.is<float>());
		assert(!v.is<double>());
		assert(v.is<int>());
		
		v = 42L;
		assert(!v.is<int>());
		assert(v.is<long>());
		
		v = 42.f;
		assert(!v.is<int>());
		assert(v.is<float>());
		
		v = 42.0;
		assert(!v.is<float>());
		assert(v.is<double>());
		
		v = false;
		assert(!v.is<string>());
		assert(v.is<bool>());
		
		v = true;
		assert(!v.is<string>());
		assert(v.is<bool>());
		
		v = "string";
		assert(!v.is<bool>());
		assert(v.is<string>());
		assert(v.isString());
		
		v = ofxValue::Array();
		assert(!v.isMap());
		assert(!v.isNull());
		assert(!v.isString());
		assert(v.isArray());
		
		v = ofxValue::Map();
		assert(!v.isNull());
		assert(!v.isString());
		assert(!v.isArray());
		assert(v.isMap());

		v.reset();
		assert(v.isNull());
		
		v = ofxValue();
		assert(v.isNull());
	}
	
	{
		// operator
		
		ofxValue v1, v2, v3;
		
		v1 = 10;
		v2 = 20;
		v3 = 20;
		
		assert(v1 != v2);
		assert(v2 == v3);
		
		v1.reset();
		v2.reset();
		
		assert(v1 == v2);
		
		assert(v3 != 10);
		assert(v3 == 20);
		
		v1 = 10;
		v2 = 20;
		v3 = 20.0f;
		
		assert(v1 < v2);
		assert(v2 > v1);
		
		assert(v2 <= v3);
		assert(v3 >= v1);
		
		assert(10 == v1);
		assert(!(10 > v1));
		assert(!(10 < v1));
		assert(10 <= v1);
		
		v1 = "test";
		v2 = "test_";
		v3 = "test";
		
		assert(v1 != v2);
		assert(v1 == v3);
		
		assert((v1 < v2) == (string("test") < string("test_")));
	}
	
	{
		// array
		
		ofxValue v = ofxValue::Array();
		v.push(10);
		v.push(20);
		v.push(30);
		
		assert(v.size() == 3);
		
		for (int i = 0; i < v.size(); i++)
		{
			assert(((i + 1) * 10) == v[i]);
		}
		
		ofxValue vv = v;
		assert(vv.size() == 3);
		
		for (int i = 0; i < v.size(); i++)
		{
			assert(v[i] == vv[i]);
		}
		
		v[1] = 42;
		assert(v[1] != vv[1]);
		
		sort(v.array().begin(), v.array().end());
		
		assert(v[1] == 30);
		
		ofxValue p = v.pop();
		assert(p == 42);
		
		ofxValue a = ofxValue::Array();
		a.push(100);
		a.push(200);
		a.push(300);
		
		v.push(a);
		
		assert(v[2].size() == 3);
		
		for (int i = 0; i < v[2].size(); i++)
		{
			assert(((i + 1) * 100) == v[2][i]);
		}
	}
	
	{
		// map
		ofxValue v = ofxValue::Map();
		
		v["key0"] = 10;
		v["key1"] = 20;
		v["key2"] = 30;
		
		assert(v.keys().size() == 3);
		assert(v["key0"] == 10);
		
		v["submap"] = ofxValue::Map();
		v["submap"]["0"] = 10;
		v["submap"]["1"] = 10;
		v["submap"]["2"] = 10;
		
		for (int i = 0; i < 3; i++)
		{
			assert(v["submap"][ofToString(i)] == 10);
		}
		
		assert(v.keys().size() == 4);
		
		v.remove("submap");
		
		assert(v.keys().size() == 3);
	}
	
	{
		// json
		
		ofxValue v = ofxValue::Map();
		v["key0"] = 10;
		v["key1"] = 20;
		v["key2"] = 30;
		
		v["array"] = ofxValue::Array();
		v["array"].push(30);
		v["array"].push(40);
		v["array"].push(50);
		v["array"].push("test");
		
		v["array"].push(ofxValue::Map());
		v["array"][4]["test1"] = 70;
		v["array"][4]["test2"] = 80;
		
		string d = v.toJson();
		cout << d << endl;
		
		ofxValue s = ofxValue::fromJson(d);
		assert(d == s.toJson());
	}
	
	cout << "TEST PASSED" << endl;
	
	ofExit();
}

