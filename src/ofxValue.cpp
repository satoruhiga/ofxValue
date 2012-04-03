#include "ofxValue.h"

#include "picojson.h"

static string to_json(ofxValue& v, int indent_level)
{
	string indent;
	
	if (v.isString()) return "\"" + v.as<string>() + "\"";
	else if (v.isBoolean()) return v.as<bool>() ? "true" : "false";
	else if (v.isInteger()) return ofToString(v.as<long>(), 0);
	else if (v.isFloatingPoint()) return ofToString(v.as<double>());
	else if (v.isNull()) return "null";
	else if (v.isArray())
	{
		string s;
		s += "[\n";
		
		indent_level++;
		indent = string(indent_level * 4, ' ');
		
		for (int i = 0; i < v.size(); i++)
		{
			s += indent + to_json(v[i], indent_level) + ",\n";
		}
		
		if (s.size() > 1)
			s.resize(s.length() - 2);
		
		indent_level--;
		indent = string(indent_level * 4, ' ');
		
		s += "\n" + indent + "]";
		return s;
	}
	else if (v.isMap())
	{
		string s;
		s += "{\n";
		
		indent_level++;
		indent = string(indent_level * 4, ' ');
		
		ofxValue::MapType::iterator it = v.map().begin();
		while (it != v.map().end())
		{
			s += indent + "\"" + (*it).first + "\": ";
			s += to_json((*it).second, indent_level) + ",\n";
			it++;
		}
		
		if (s.size() > 1)
			s.resize(s.length() - 2);
		
		indent_level--;
		indent = string(indent_level * 4, ' ');
		
		s += "\n" + indent + "}";
		return s;
	}
	
	return "";
}

string ofxValue::toJSON()
{
	return to_json(*this, 0);
}

using namespace picojson;

static ofxValue from_json(picojson::value j)
{
	ofxValue v;
	
	if (j.is<array>())
	{
		v = ofxValue::Array();
		
		array arr = j.get<array>();
		for (array::iterator it = arr.begin(); it != arr.end(); it++)
		{
			 v.push(from_json((*it)));
		}
	}
	else if (j.is<object>())
	{
		v = ofxValue::Map();
		
		object obj = j.get<object>();
		for (object::iterator it = obj.begin(); it != obj.end(); it++)
		{
			v[(*it).first] = from_json((*it).second);
		}
	}
	else if (j.is<bool>()) v = j.get<bool>();
	else if (j.is<int>()) v = (int)j.get<double>();
	else if (j.is<double>()) v = j.get<double>();
	else if (j.is<string>()) v = j.get<string>();
	else if (j.is<null>()) v = ofxValue();
	
	return v;
}

ofxValue ofxValue::fromJSON(string json)
{
	value j;
	string err;
	parse(j, json.begin(), json.end(), &err);
	
	if (!err.empty())
	{
		ofLogError("ofxValue", "json parse error");
		cout << err << endl;
		return ofxValue();
	}
	
	return from_json(j);
}