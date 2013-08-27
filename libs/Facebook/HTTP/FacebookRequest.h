/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * FacebookRequest.h
 */

#ifndef FACEBOOKREQUEST_H_
#define FACEBOOKREQUEST_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>

#include "HttpRequest.h"

using namespace MAUtil;

#define FACEBOOK_OPEN_GRAPH_URLBASE "https://graph.facebook.com/"

enum FACEBOOK_REQUEST_TYPE { RETRIEVE_DATA = 0, PUBLISH = 1 };
enum FACEBOOK_RESPONSE_TYPE { JSON = 2, IMAGE = 3, VIDEO = 4, STRING = 5, BOOLEAN = 6 };

class FacebookRequest : public HttpRequest {
public:
	FacebookRequest(FACEBOOK_REQUEST_TYPE reqType,
					FACEBOOK_RESPONSE_TYPE responseType,
					const String& path,
					String& accessToken,
					int method,
					const String postdata = "");

	virtual const String& getPath() const;

	virtual FACEBOOK_RESPONSE_TYPE getResponseType() const;
	virtual FACEBOOK_REQUEST_TYPE getType() const;

	inline virtual ~FacebookRequest() {}

protected:
	String mPath;
	FACEBOOK_REQUEST_TYPE mType;
	FACEBOOK_RESPONSE_TYPE mResponseType;
};

#endif /* FACEBOOKREQUEST_H_ */
