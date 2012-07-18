#pragma once

#include "ofMain.h"

class ofxValue
{
protected:

	enum Type
	{
		NULL_TYPE,
		FLOAT_TYPE,
		DOUBLE_TYPE,
		INT32_TYPE,
		INT64_TYPE,
		STRING_TYPE,
		BOOLEAN_TYPE,
		MAP_TYPE,
		ARRAY_TYPE,
		// BINARY_TYPE, // TODO
	};

public:
	
	typedef vector<ofxValue> ArrayType;
	typedef map<string, ofxValue> MapType;
	
	static ofxValue Array() { return ofxValue(ARRAY_TYPE); }
	static ofxValue Map() { return ofxValue(MAP_TYPE); }

	inline ofxValue() : type(NULL_TYPE), data_(NULL) {}
	
	inline ofxValue(const ofxValue& copy);
	inline ofxValue& operator=(const ofxValue& copy);
	
	inline ofxValue(const bool& v) : type(BOOLEAN_TYPE), number_(v) {}
	inline ofxValue(const int& v) : type(INT32_TYPE), number_(v) {}
	inline ofxValue(const long& v) : type(INT64_TYPE), number_(v) {}
	inline ofxValue(const float& v) : type(FLOAT_TYPE), number_(v) {}
	inline ofxValue(const double& v) : type(DOUBLE_TYPE), number_(v) {}
	inline ofxValue(const char* v) : type(STRING_TYPE), string_(new string(v)) {}
	inline ofxValue(const string& v) : type(STRING_TYPE), string_(new string(v)) {}
	
	inline ~ofxValue() { clear(); }
	
	inline void reset() { clear(); }
	
	template <class T> inline T as() const {}
	template <class T> inline bool is() const {}
	
	inline bool isNull() const { return type == NULL_TYPE; }
	inline bool isNumber() const;
	inline bool isInteger() const { return type == INT32_TYPE || type == INT64_TYPE; }
	inline bool isFloatingPoint() const { return type == FLOAT_TYPE || type == DOUBLE_TYPE; }
	inline bool isBoolean() const { return type == BOOLEAN_TYPE; }
	inline bool isString() const { return type == STRING_TYPE; }
	inline bool isArray() const { return type == ARRAY_TYPE; }
	inline bool isMap() const { return type == MAP_TYPE; }

	inline const size_t size() const;
	
	// Array utilities
	
	inline ArrayType& array() { assert(isArray()); return *array_; }
	
	inline ofxValue& operator[](size_t index);
	
	inline ofxValue& push(const ofxValue& v);
	inline ofxValue pop();
	inline ofxValue remove(size_t index);
	

	// Map utilities
	
	inline MapType& map() { assert(isMap()); return *map_; }
	
	inline ofxValue& operator[](const string& key);
	
	inline ofxValue remove(const string &key);
	inline bool hasKey(const string& key) const;
	
	inline vector<string> keys() const;
	inline vector<ofxValue> values() const;
	
	// operators
	
	friend inline bool operator==(const ofxValue& v1, const ofxValue& v2)
	{
		if (v1.isNull() && v2.isNull()) return true;
		else if (v1.isNumber() && v2.isNumber()) return v1.number_ == v2.number_;
		else if (v1.isString() && v2.isString()) return *v1.string_ == *v2.string_;
		throw;
	}
	
	friend inline bool operator<(const ofxValue& v1, const ofxValue& v2)
	{
		if (v1.isNumber() && v2.isNumber()) return v1.number_ < v2.number_;
		else if (v1.isString() && v2.isString()) return *v1.string_ < *v2.string_;
		throw;
	}
	
	friend inline bool operator>(const ofxValue& v1, const ofxValue& v2)
	{
		if (v1.isNumber() && v2.isNumber()) return v1.number_ > v2.number_;
		else if (v1.isString() && v2.isString()) return *v1.string_ > *v2.string_;
		throw;
	}

	template<typename T> 
	friend inline bool operator==(const T& o, const ofxValue& v) { return v.as<T>() == o; }
	template<typename T> 
	friend inline bool operator!=(const T& o, const ofxValue& v) { return !operator==(o, v); }
	template<typename T> 
	friend inline bool operator>=(const T& o, const ofxValue& v) { return !operator<(o, v); }
	template<typename T> 
	friend inline bool operator>(const T& o, const ofxValue& v) { return v.as<T>() > o; }
	template<typename T> 
	friend inline bool operator<(const T& o, const ofxValue& v) { return v.as<T>() < o; }
	template<typename T> 
	friend inline bool operator<=(const T& o, const ofxValue& v) { return !operator>(o, v); }
	
	// json serialization
	
	string toJSON();
	static bool fromJSON(string json, ofxValue& value);
	
protected:

	Type type;
	
	union
	{
		double number_;
		string *string_;
		ArrayType *array_;
		MapType *map_;
		void *data_;
	};
	
	inline ofxValue(Type t) { init(t); }
	inline ofxValue(const ArrayType& v) : type(ARRAY_TYPE) { array_ = new ArrayType(v); }
	inline ofxValue(const MapType& v) : type(MAP_TYPE) { map_ = new MapType(v); }

	inline void init(Type t);
	inline void clear();
};



#pragma mark - inline

inline ofxValue ofxValue::remove(size_t index)
{
	assert(isArray());
	
	ofxValue v = array_->at(index);
	array_->erase(array_->begin() + index);
	return v;
}

inline ofxValue ofxValue::remove(const string &key)
{
	assert(isMap());
	
	ofxValue v = (*map_)[key];
	map_->erase(key);
	
	return v;
}

inline ofxValue& ofxValue::push(const ofxValue& v)
{
	assert(isArray());
	array_->push_back(v);
	return *this;
}

inline ofxValue ofxValue::pop()
{
	assert(isArray());
	ofxValue v = array_->back();
	array_->pop_back();
	return v;
}

inline vector<string> ofxValue::keys() const
{
	assert(isMap());

	vector<string> v;
	MapType::iterator it = map_->begin();

	while (it != map_->end())
	{
		v.push_back((*it).first);
		it++;
	}

	return v;
}

inline vector<ofxValue> ofxValue::values() const
{
	assert(isMap());
	
	vector<ofxValue> v;
	MapType::iterator it = map_->begin();
	
	while (it != map_->end())
	{
		v.push_back((*it).second);
		it++;
	}
	
	return v;
}

inline bool ofxValue::isNumber() const
{
	return type == FLOAT_TYPE
	|| type == DOUBLE_TYPE
	|| type == INT32_TYPE
	|| type == INT64_TYPE
	|| type == BOOLEAN_TYPE;
}

inline ofxValue& ofxValue::operator[](size_t index)
{
	assert(isArray()); // TODO: exception
	return array_->at(index);
}

inline ofxValue& ofxValue::operator[](const string& key)
{
	assert(isMap()); // TODO: exception
	return (*map_)[key];
}

inline const size_t ofxValue::size() const
{
	assert(isArray() || isMap());
	if (isArray()) return array_->size();
	else if (isMap()) return map_->size();
}

inline bool ofxValue::hasKey(const string& key) const
{
	assert(isMap());
	return map_->find(key) != map_->end();
}

inline ofxValue::ofxValue(const ofxValue& copy)
{
	data_ = NULL;
	type = copy.type;
#define COPY(t, d, s) case t: d = s; break;
	switch (type) {
			COPY(FLOAT_TYPE, number_, copy.number_);
			COPY(DOUBLE_TYPE, number_, copy.number_);
			COPY(INT32_TYPE, number_, copy.number_);
			COPY(INT64_TYPE, number_, copy.number_);
			COPY(BOOLEAN_TYPE, number_, copy.number_);
			COPY(STRING_TYPE, string_, new string(*copy.string_));
			COPY(ARRAY_TYPE, array_, new ArrayType(*copy.array_));
			COPY(MAP_TYPE, map_, new MapType(*copy.map_));
		default:
			break;
	}
#undef COPY
}

inline ofxValue& ofxValue::operator=(const ofxValue& copy)
{
	clear();
	type = copy.type;
#define COPY(t, d, s) case t: d = s; break;
	switch (type) {
		COPY(FLOAT_TYPE, number_, copy.number_);
		COPY(DOUBLE_TYPE, number_, copy.number_);
		COPY(INT32_TYPE, number_, copy.number_);
		COPY(INT64_TYPE, number_, copy.number_);
		COPY(BOOLEAN_TYPE, number_, copy.number_);
		COPY(STRING_TYPE, string_, new string(*copy.string_));
		COPY(ARRAY_TYPE, array_, new ArrayType(*copy.array_));
		COPY(MAP_TYPE, map_, new MapType(*copy.map_));
		default:
			break;
	}
#undef COPY
	return *this;
}

inline void ofxValue::init(Type t)
{
	type = t;
#define INIT(t, v, i) case t: v = i; break;
	switch (type) {
		case FLOAT_TYPE:
		case DOUBLE_TYPE:
		case INT32_TYPE:
		case INT64_TYPE:
		case BOOLEAN_TYPE:
			number_ = 0;
			break;
		INIT(STRING_TYPE, string_, new string());
		INIT(ARRAY_TYPE, array_, new ArrayType());
		INIT(MAP_TYPE, map_, new MapType());
		default:
			break;
	}
#undef INIT
}

inline void ofxValue::clear()
{
#define FREE(t, p) case t: if (p) { delete p; p = NULL; } break;
	switch (type) {
		FREE(STRING_TYPE, string_);
		FREE(ARRAY_TYPE, array_);
		FREE(MAP_TYPE, map_);
		default:
			break;
	}
#undef FREE
	
	type = NULL_TYPE;
	data_ = NULL;
}

template<> inline double ofxValue::as() const
{
	if (isNumber()) return number_;
	else if (isString()) return ofToFloat(*string_);

	ofLogError("ofxValue", "invalid cast");
	return 0;
}

template<> inline float ofxValue::as() const
{
	if (isNumber()) return number_;
	else if (isString()) return ofToFloat(*string_);
	
	ofLogError("ofxValue", "invalid cast");
	return 0;
}

template<> inline int ofxValue::as() const
{
	if (isNumber()) return number_;
	else if (isString()) return ofToInt(*string_);
	
	ofLogError("ofxValue", "invalid cast");
	return 0;
}

template<> inline long ofxValue::as() const
{
	if (isNumber()) return number_;
	else if (isString()) return ofToInt(*string_);
	
	ofLogError("ofxValue", "invalid cast");
	return 0;
}

template<> inline bool ofxValue::as() const
{
	if (isNull()) return false;
	else if (isNumber()) return number_;
	else if (isString()) return *string_ != "";
	else if (isArray()) return !array_->empty();
	else if (isMap()) return !map_->empty();
	
	ofLogError("ofxValue", "invalid cast");
	return false;
}

template<> inline string ofxValue::as() const
{
	if (isString()) return *string_;
	else if (isInteger()) return ofToString(number_, 0);
	else if (isFloatingPoint()) return ofToString(number_);
	else if (isNull()) return "(null)";
	else if (isArray()) return "(Array)";
	else if (isMap()) return "(Map)";
	
	ofLogError("ofxValue", "invalid cast");
	return "";
}

template<> inline bool ofxValue::is<float>() const { return type == FLOAT_TYPE; }
template<> inline bool ofxValue::is<double>() const { return type == DOUBLE_TYPE; }
template<> inline bool ofxValue::is<int>() const { return type == INT32_TYPE; }
template<> inline bool ofxValue::is<long>() const { return type == INT64_TYPE; }
template<> inline bool ofxValue::is<bool>() const { return type == BOOLEAN_TYPE; }
template<> inline bool ofxValue::is<string>() const { return type == STRING_TYPE; }
template<> inline bool ofxValue::is<ofxValue::ArrayType>() const { return type == ARRAY_TYPE; }
template<> inline bool ofxValue::is<ofxValue::MapType>() const { return type == MAP_TYPE; }
