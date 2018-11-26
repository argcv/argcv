/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Yu Jing
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#ifndef ARGCV_CXX_ENCODING_XML_XML_SAX_PARSER_H_
#define ARGCV_CXX_ENCODING_XML_XML_SAX_PARSER_H_
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <functional>
#include <map>
#include <memory>
#include <mutex>  // NOLINT(build/c++11)  std::once_flag
#include <string>
#include <utility>
#include <vector>

#include "glog/logging.h"
#include "libxml/SAX2.h"
#include "libxml/tree.h"

namespace argcv {

std::once_flag xml_parser_init_ca_flag;

class XmlDocDecl {
 public:
  explicit XmlDocDecl(const std::string &version = "1.0")
      : doc_decl_(xmlNewDoc((const xmlChar *)version.c_str())),
        ext_subset_(nullptr) {
    std::call_once(xml_parser_init_ca_flag, []() {
      LOG(INFO) << "[call_once] called: xmlInitParser";
      xmlInitParser();
    });
    doc_decl_->_private = this;
  }

  explicit XmlDocDecl(xmlDocPtr doc) : doc_decl_(doc), ext_subset_(nullptr) {
    std::call_once(xml_parser_init_ca_flag, []() {
      LOG(INFO) << "[call_once] called: xmlInitParser";
      xmlInitParser();
    });
    doc_decl_->_private = this;
  }

  virtual ~XmlDocDecl() {
    if (ext_subset_) {
      xmlFreeDtd(ext_subset_);
      doc_decl_->extSubset = nullptr;
    }
    xmlFreeDoc(doc_decl_);
  }

  static xmlDtdPtr ParseSubset(const std::string &external,
                               const std::string &system_id) noexcept {
    auto dtd = xmlParseDTD(
        external.empty() ? nullptr : (const xmlChar *)external.c_str(),
        system_id.empty() ? nullptr : (const xmlChar *)system_id.c_str());
    if (!dtd) {
      LOG(ERROR) << "DTD " << external << "#" << system_id << " parsed failed ";
    }
    return dtd;
  }

  // for external
  virtual void SetExternalSubset(const std::string &name,
                                 const std::string &external_id,
                                 const std::string &system_id) noexcept {
    assert(doc_decl_ != nullptr);
    LOG(INFO) << "[SetExternalSubset] loading..[name]" << name
              << "#[external_id]" << external_id << "#[system_id]" << system_id;
    // dtd_ = xmlCreateIntSubset(
    //     doc_decl_, (const xmlChar *)name.c_str(),
    //     external_id.empty() ? nullptr : (const xmlChar *)external_id.c_str(),
    //     system_id.empty() ? nullptr : (const xmlChar *)system_id.c_str());

    ext_subset_ = ParseSubset(external_id, GetSystemURIMapping(system_id));

    if (doc_decl_->extSubset != nullptr) {
      xmlFreeDtd(doc_decl_->extSubset);
    }
    doc_decl_->extSubset = ext_subset_;

    LOG(INFO) << "[SetExternalSubset] ready..[name]" << name << "#[external_id]"
              << external_id << "#[system_id]" << system_id;
  }

  virtual xmlEntityPtr GetEntity(const std::string &name) noexcept {
    return xmlGetDocEntity(doc_decl_, (const xmlChar *)name.c_str());
  }

  virtual void AddSystemURIMapping(const std::string &from,
                                   const std::string &to) noexcept {
    system_id_uri_mapping_.insert(
        std::pair<const std::string, const std::string>(from, to));
  }

  virtual const std::string GetSystemURIMapping(
      const std::string &from) noexcept {
    if (system_id_uri_mapping_.find(from) == system_id_uri_mapping_.end()) {
      return from;
    } else {
      return system_id_uri_mapping_[from];
    }
  }

  virtual xmlDocPtr UnsafePtr() noexcept { return doc_decl_; }

  std::map<const std::string, const std::string> &system_id_uri_mapping() {
    return system_id_uri_mapping_;
  }

 protected:
  std::map<const std::string, const std::string> system_id_uri_mapping_;

 private:
  xmlDocPtr doc_decl_;
  xmlDtdPtr ext_subset_;
};

typedef struct _xmlAttrElem {
  const std::string localname;
  const std::string prefix;
  const std::string uri;
  const std::string value;
  bool defaulted;
} XmlAttrElem;
typedef std::vector<XmlAttrElem> XmlAttrElems;
typedef std::pair<const std::string, const std::string> XmlAttrPair;
typedef std::vector<XmlAttrPair> XmlAttrPairs;

typedef std::function<void(const std::string &localname,
                           const std::string &prefix, const std::string &uri,
                           const XmlAttrPairs &nss, const XmlAttrElems &attrs)>
    FuncOnStartElementNs;

typedef std::function<void(const std::string &, const std::string &,
                           const std::string &)>
    FuncOnEndElementNs;

typedef std::function<void(const std::string &name, const XmlAttrPairs &attrs)>
    FuncOnStartElement;

typedef std::function<void(const std::string &)> FuncBuffIn;

typedef FuncBuffIn FuncOnEndElement;
typedef FuncBuffIn FuncOnCharacters;
typedef FuncBuffIn FuncOnCdataBlock;
typedef FuncBuffIn FuncOnLog;

// ref: http://xmlsoft.org/html/libxml-tree.html#xmlSAXHandler
class XmlSAXParser {
 public:
  XmlSAXParser() : handler_(new xmlSAXHandler), xml_parse_option_(0) {
    xmlSAXHandler temp = {
        nullptr,                         // internalSubset
        nullptr,                         // isStandalone
        nullptr,                         // hasInternalSubset
        nullptr,                         // hasExternalSubset
        nullptr,                         // resolveEntity
        XmlSAXParser::onGetEntity,       // getEntity
        nullptr,                         // entityDecl
        nullptr,                         // notationDecl
        nullptr,                         // attributeDecl
        nullptr,                         // elementDecl
        nullptr,                         // unparsedEntityDecl
        nullptr,                         // setDocumentLocator
        nullptr,                         // startDocument
        nullptr,                         // endDocument
        nullptr,                         // startElement
        nullptr,                         // endElement
        nullptr,                         // reference
        nullptr,                         // characters
        nullptr,                         // ignorableWhitespace
        nullptr,                         // processingInstruction
        nullptr,                         // comment
        nullptr,                         // warning
        nullptr,                         // error
        nullptr,                         // fatalError
        nullptr,                         // getParameterEntity
        nullptr,                         // cdataBlock
        XmlSAXParser::onExternalSubset,  // externalSubset
        XML_SAX2_MAGIC,                  // initialized
        this,                            // private
        nullptr,                         // startElementNs
        nullptr,                         // endElementNs
        nullptr,                         // serror
    };
    *handler_ = temp;
    InitXmlDocDecl(true);
    // LOG(INFO) << "init: XmlSAXParser finished";
  }

  virtual ~XmlSAXParser() {
    // LOG(INFO) << "destroy: XmlSAXParser";
  }

  virtual XmlSAXParser &OnStartElementNs(FuncOnStartElementNs func) {
    func_on_start_element_ns_ = func;
    handler_->startElementNs = &(XmlSAXParser::onStartElementNs);
    return *this;
  }

  virtual XmlSAXParser &OnEndElementNs(FuncOnEndElementNs func) {
    func_on_end_element_ns_ = func;
    handler_->endElementNs = &(XmlSAXParser::onEndElementNs);
    return *this;
  }

  virtual XmlSAXParser &OnStartElement(FuncOnStartElement func) {
    func_on_start_element_ = func;
    handler_->startElement = &(XmlSAXParser::onStartElement);
    return *this;
  }

  virtual XmlSAXParser &OnEndElement(FuncOnEndElement func) {
    func_on_end_element_ = func;
    handler_->endElement = &(XmlSAXParser::onEndElement);
    return *this;
  }

  virtual XmlSAXParser &OnCharacters(FuncOnCharacters func) {
    func_on_characters_ = func;
    handler_->characters = &(XmlSAXParser::onCharacters);
    return *this;
  }

  virtual XmlSAXParser &OnCdataBlock(FuncOnCharacters func) {
    func_on_cdata_block_ = func;
    handler_->cdataBlock = &(XmlSAXParser::onCdataBlock);
    return *this;
  }

  virtual XmlSAXParser &OnComment(FuncOnLog func) {
    func_on_comment_ = func;
    handler_->comment = &(XmlSAXParser::onComment);
    return *this;
  }

  virtual XmlSAXParser &OnWarning(FuncOnLog func) {
    func_on_warning_ = func;
    handler_->warning = &(XmlSAXParser::onWarning);
    return *this;
  }

  virtual XmlSAXParser &OnError(FuncOnLog func) {
    func_on_error_ = func;
    handler_->error = &(XmlSAXParser::onError);
    return *this;
  }

  virtual XmlSAXParser &OnFatal(FuncOnLog func) {
    func_on_fatal_ = func;
    handler_->fatalError = &(XmlSAXParser::onFatal);
    return *this;
  }

  virtual XmlSAXParser &InitXmlDocDecl(bool reserve_mapping) {
    // TODO(yuikns): use option 'reserve_mapping'
    xml_doc_decl_.reset(new XmlDocDecl);
    return *this;
  }

  // ref: http://xmlsoft.org/html/libxml-parser.html#xmlParserOption
  virtual XmlSAXParser &Option(int opt) {
    xml_parse_option_ |= opt;
    return *this;
  }

  virtual XmlSAXParser &ResetOption() {
    xml_parse_option_ = 0;
    return *this;
  }

  virtual XmlSAXParser &AddDTDSystemURIMapping(const std::string &from,
                                               const std::string &to) {
    xml_doc_decl_->AddSystemURIMapping(from, to);
    return *this;
  }

  virtual const std::string GetDTDSystemURIMapping(const std::string &from) {
    return xml_doc_decl_->GetSystemURIMapping(from);
  }

  int ReadFile(FILE *f, const char *filename) {
    LOG(INFO) << "[ReadFile] starting...";
    xmlResetLastError();
    char chars[1024];
    int res = 0;
    xmlParserCtxtPtr ctxt =
        xmlCreatePushParserCtxt(handler_.get(), this, chars, res, filename);
    if (ctxt != nullptr) {
      if (xmlCtxtUseOptions(ctxt, xml_parse_option_)) {
        LOG(FATAL) << "OPTION SET FAILED..";
      }
      while ((res = fread(chars, 1, sizeof(chars), f)) > 0) {
        if (xmlParseChunk(ctxt, chars, res, 0)) {
          // return 1;
        }
      }
      xmlParseChunk(ctxt, chars, 0, 1);
      xmlFreeParserCtxt(ctxt);
    } else {
      LOG(ERROR) << "ctxt == nullptr !!!";
    }
    xmlCleanupParser();
    return 0;
  }

  int ReadString(const std::string &data, const char *filename) {
    xmlResetLastError();
    int n = data.size();
    int res = 0;
    char chars[1024];
    memset(chars, 0, 1024);
    xmlParserCtxtPtr ctxt =
        xmlCreatePushParserCtxt(handler_.get(), this, chars, res, filename);
    if (ctxt != nullptr) {
      for (int i = 0; i < n; i += 1024) {
        memset(chars, 0, 1024);
        res = n - i;
        int len = res > 1024 ? 1024 : res;
        memcpy(chars, data.data() + i, len);
        if (xmlParseChunk(ctxt, chars, len, (res > 1024 ? 0 : 1))) {
          // return 1;
          LOG(ERROR) << "xmlParseChunk... errored"
                     << "res:" << res;
        }
      }
      // memset(chars, 0, 1024);
      // xmlParseChunk(ctxt, chars, 0, 1);
      xmlFreeParserCtxt(ctxt);
    } else {
      LOG(ERROR) << "ctxt == nullptr !!!";
    }
    xmlCleanupParser();
    return 0;
  }

 protected:
  std::unique_ptr<xmlSAXHandler> handler_;
  // used to load DTDs
  std::unique_ptr<XmlDocDecl> xml_doc_decl_;

  FuncOnStartElementNs func_on_start_element_ns_;
  FuncOnEndElementNs func_on_end_element_ns_;
  FuncOnStartElement func_on_start_element_;
  FuncOnEndElement func_on_end_element_;
  FuncOnCharacters func_on_characters_;
  FuncOnCdataBlock func_on_cdata_block_;
  FuncOnLog func_on_comment_;
  FuncOnLog func_on_warning_;
  FuncOnLog func_on_error_;
  FuncOnLog func_on_fatal_;  //  unused
  int xml_parse_option_;

  // macros start
  static std::string errorStringFormat(const char *msg, va_list arg) {
    va_list arg_c;
    va_copy(arg_c, arg);
    const int nchar = std::vsnprintf(nullptr, 0, msg, arg_c);
    va_end(arg_c);
    std::string sbuf;
    if (nchar > 0) {
      std::vector<char> buf(nchar + 1);
      std::vsnprintf(buf.data(), buf.size(), msg, arg);
      sbuf.assign(buf.data(), buf.size());
    }
    return sbuf;
  }

  static std::string xmlChar2Str(const xmlChar *xml_str) {
    static std::string blank;
    if (xml_str == nullptr) {
      return blank;
    } else {
      return std::string((const char *)xml_str);
    }
  }

  // macros end

  static void onStartElementNs(void *ctx, const xmlChar *localname,
                               const xmlChar *prefix, const xmlChar *uri,
                               int nb_namespaces, const xmlChar **namespaces,
                               int nb_attributes, int nb_defaulted,
                               const xmlChar **attributes) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      std::vector<std::pair<const std::string, const std::string>> ns;
      for (int i = 0; i < nb_namespaces; i++) {
        const std::string k = xmlChar2Str(namespaces[2 * i]);
        const std::string v = xmlChar2Str(namespaces[2 * i + 1]);
        ns.push_back(std::pair<const std::string, const std::string>(k, v));
      }
      XmlAttrElems attrs;
      for (int i = 0; i < nb_attributes; i++) {
        const std::string localname = xmlChar2Str(attributes[5 * i]);
        const std::string prefix = xmlChar2Str(attributes[5 * i + 1]);
        const std::string uri = xmlChar2Str(attributes[5 * i + 2]);
        const char *value_start = (const char *)attributes[5 * i + 3];
        const char *value_end = (const char *)attributes[5 * i + 4];
        size_t len = value_end - value_start;
        const std::string value = std::string(value_start, len);
        bool defaulted = i > (nb_attributes - nb_defaulted);

        XmlAttrElem attr = {
            localname,  // localname
            prefix,     // prefix
            uri,        // uri
            value,      // value
            defaulted   // defaulted
        };
        attrs.push_back(attr);
      }
      p->func_on_start_element_ns_(xmlChar2Str(localname), xmlChar2Str(prefix),
                                   xmlChar2Str(uri), ns, attrs);
    }
  }

  static void onEndElementNs(void *ctx, const xmlChar *localname,
                             const xmlChar *prefix, const xmlChar *uri) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      p->func_on_end_element_ns_(xmlChar2Str(localname), xmlChar2Str(prefix),
                                 xmlChar2Str(uri));
    }
  }

  static void onStartElement(void *ctx, const xmlChar *name,
                             const xmlChar **attributes) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      // An array of name/value attributes pairs, NULL terminated
      std::vector<std::pair<const std::string, const std::string>> attrs;
      for (const xmlChar **cattr = attributes; cattr && *cattr; cattr += 2) {
        const std::string k = xmlChar2Str(*cattr);
        const std::string v = xmlChar2Str(*(cattr + 1));
        attrs.push_back(std::pair<const std::string, const std::string>(k, v));
      }
      p->func_on_start_element_(xmlChar2Str(name), attrs);
    }
  }

  static void onEndElement(void *ctx, const xmlChar *name) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      p->func_on_end_element_(xmlChar2Str(name));
    }
  }

  static void onCharacters(void *ctx, const xmlChar *ch, int len) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      std::vector<char> buf(len);
      memcpy(buf.data(), ch, sizeof(char) * len);
      std::string sbuff(buf.data(), buf.size());
      p->func_on_characters_(sbuff);
    }
  }

  static void onCdataBlock(void *ctx, const xmlChar *ch, int len) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      std::vector<char> buf(len);
      memcpy(buf.data(), ch, sizeof(char) * len);
      std::string sbuff(buf.data(), buf.size());
      p->func_on_cdata_block_(sbuff);
    }
  }

  static void onComment(void *ctx, const xmlChar *value) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      p->func_on_comment_(xmlChar2Str(value));
    }
  }

  static void onWarning(void *ctx, const char *msg, ...) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      va_list arg;
      va_start(arg, msg);
      std::string sbuf(errorStringFormat(msg, arg));
      va_end(arg);
      p->func_on_warning_(sbuf);
    }
  }

  static void onError(void *ctx, const char *msg, ...) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      va_list arg;
      va_start(arg, msg);
      std::string sbuf(errorStringFormat(msg, arg));
      va_end(arg);
      p->func_on_error_(sbuf);
    }
  }

  static void onFatal(void *ctx, const char *msg, ...) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      va_list arg;
      va_start(arg, msg);
      std::string sbuf(errorStringFormat(msg, arg));
      va_end(arg);
      p->func_on_fatal_(sbuf);
    }
  }

  static void onExternalSubset(void *ctx, const xmlChar *name,
                               const xmlChar *external_id,
                               const xmlChar *system_id) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      const std::string root_name = xmlChar2Str(name);
      const std::string eid = xmlChar2Str(external_id);
      const std::string sid = xmlChar2Str(system_id);
      // LOG(INFO) << "starting to set entity decl";
      p->xml_doc_decl_->SetExternalSubset(root_name, eid, sid);
    }
  }

  static xmlEntityPtr onGetEntity(void *ctx, const xmlChar *name) {
    auto p = static_cast<XmlSAXParser *>(ctx);
    if (p != nullptr) {
      const std::string entity_name = xmlChar2Str(name);
      // LOG(INFO) << "[onGetEntity:q] name:" << entity_name;
      auto entity_ptr = p->xml_doc_decl_->GetEntity(entity_name);
      // LOG(INFO) << "[onGetEntity:r] name:" << entity_name << " : "
      //           << entity_ptr;
      return entity_ptr;
    } else {
      LOG(ERROR) << "[onGetEntity] failed p not found!";
      return nullptr;
    }
  }
};

}  // namespace argcv

#endif  // ARGCV_CXX_ENCODING_XML_XML_SAX_PARSER_H_
