/*
 * SearchDescription.cpp
 *
 *  Created on: 25 Apr 2016
 *      Author: adamfowler
 */

#include "SearchDescription.hpp"
#include "DocumentContent.hpp"
#include "InvalidFormatException.hpp"
#include <string>
#include <iostream>

#include "easylogging++.h"

namespace mlclient {

class SearchDescription::Impl {
public:
  Impl() {
    TextDocumentContent* qtdc = new TextDocumentContent();
    query = std::unique_ptr<TextDocumentContent>(std::move(qtdc));
    TextDocumentContent* otdc = new TextDocumentContent();
    options = std::unique_ptr<TextDocumentContent>(std::move(otdc));
    std::string* qt = new std::string("");
    queryText = std::unique_ptr<std::string>(std::move(qt));
  }

  ~Impl() {
    ;
  }

  std::unique_ptr<TextDocumentContent> query;
  std::unique_ptr<TextDocumentContent> options;
  std::unique_ptr<std::string> queryText;
}; // end SearchDescription::Impl class


SearchDescription::SearchDescription() : mImpl(new Impl) {
  ;
}

SearchDescription::~SearchDescription() {
  delete mImpl;
  mImpl = NULL;
}


void SearchDescription::setOptions(TextDocumentContent& options) {
  mImpl->options = std::unique_ptr<TextDocumentContent>(std::move(&options)); // TODO check this & works
}
const TextDocumentContent& SearchDescription::getOptions() const {
  return *(mImpl->options.get());
}
void SearchDescription::setQuery(TextDocumentContent& query) {
  mImpl->query = std::unique_ptr<TextDocumentContent>(std::move(&query)); // TODO check this & works
}
const TextDocumentContent& SearchDescription::getQuery() const {
  return *(mImpl->query.get());
}
void SearchDescription::setQueryText(std::string qtext) {
  mImpl->queryText = std::unique_ptr<std::string>(new std::string(qtext));
}
const std::string& SearchDescription::getQueryText() const {
  return *(mImpl->queryText.get());
}
TextDocumentContent* SearchDescription::getPayload() const {
  TIMED_FUNC(SearchDescription_getPayload);
  LOG(DEBUG) << "    Entering getPayload()";
  // if options has not been initialised, leave blank, but set mime type as same as query
  if (mImpl->options.get()->getMimeType().empty()) {
    mImpl->options.get()->setMimeType(mImpl->query.get()->getMimeType());
  }
  LOG(DEBUG) << "    set mime type";
  // we could also have options, but no query (using qtext instead)
  if (mImpl->query.get()->getMimeType().empty()) {
    mImpl->query.get()->setMimeType(mImpl->options.get()->getMimeType());
  }
  LOG(DEBUG) << "    set query";
  // or just a blank query and options!
  if (mImpl->options.get()->getMimeType().empty() && mImpl->query.get()->getMimeType().empty()) {
    mImpl->options.get()->setMimeType(DocumentContent::MIME_JSON);
    mImpl->query.get()->setMimeType(DocumentContent::MIME_JSON);
  }
  LOG(DEBUG) << "    reset mime type";
  if ( !(0==DocumentContent::MIME_JSON.compare(mImpl->query.get()->getMimeType()) && 0==DocumentContent::MIME_JSON.compare(mImpl->options.get()->getMimeType()))
       &&
       !(0==DocumentContent::MIME_XML.compare(mImpl->query.get()->getMimeType()) && 0==DocumentContent::MIME_XML.compare(mImpl->options.get()->getMimeType()))
     ) {

    LOG(DEBUG) << "    MIME TYPES DO NOT MATCH - THROWING EXCEPTION: Query mime: " << mImpl->query.get()->getMimeType()
              << ", options MIME: " << mImpl->options.get()->getMimeType();
    throw new InvalidFormatException;
  }
  std::string elements[8] = {
      "{\"search\": {", "<search>",
      "}}","</search>",
      "\"qtext\": \"","<qtext>",
      "\"", "</qtext>"
  };
  LOG(DEBUG) << "    got elements";
  int offset = 0; // default to JSON
  if (0==DocumentContent::MIME_XML.compare(mImpl->query.get()->getMimeType())) {
    offset = 1;
  }
  std::string payloadString = elements[0+offset] + mImpl->query.get()->getContent() + mImpl->options.get()->getContent() + elements[4+offset] +
      *(mImpl->queryText.get()) + elements[6+offset] + elements[2+offset];
  LOG(DEBUG) << "    got payload string";
  TextDocumentContent* payload = new TextDocumentContent;
  payload->setMimeType(mImpl->query.get()->getMimeType());
  payload->setContent(std::move(payloadString));
  LOG(DEBUG) << "    got payload doc";
  LOG(DEBUG) << "    End getPayload()";
  return payload;
}

} // end namespace mlclient
