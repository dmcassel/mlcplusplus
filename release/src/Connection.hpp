/**
 * \file Connection.hpp
 * \author Adam Fowler <adam.fowler@marklogic.com>
 * \since 8.0.0
 * \date 2015-11-29
 *
 * \brief Provides the definition of the \link Connection \endlink class. No enums or constants are defined
 *
 */
#ifndef __MLCLIENT_CONNECTION__
#define __MLCLIENT_CONNECTION__

#include "Response.hpp"
#include "DocumentContent.hpp"
#include "SearchDescription.hpp"

/**
 * \brief the namespace which wraps all Core Public C++ API classes.
 */
namespace mlclient {

/**
 * \class Connection
 * \author Adam Fowler <adam.fowler@marklogic.com>
 * \since 8.0.0
 * \date 2015-11-29
 *
 * \brief Provides a high level logical object representing all access to a single MarkLogic database
 *
 * Wraps all underlying TCP, HTTP and authentication provided by the internal API. Provides convenience
 * methods that map 1:1 with MarkLogic Server's REST API.
 *
 *
 */
class Connection {

public:
  /**
   * \brief Creates a Connection instance.
   *
   * Defaults connection to admin/admin, localhost, nossl, port 8002, and the Documents database.
   *
   * \since 8.0.0
   */
  Connection();
  /**
   * Destroys a Connection instance.
   */
  ~Connection();

  /// \name connection_configuration Connection configuration functions
  // @{

  /**
   * Configures this connection, providing connection override information - host, port, username, password and SSL usage
   *
   * \since 8.0.0
   *
   * \param[in] hostname The hostname or IP address of the MarkLogic Server (or load balancer) to connect to
   * \param[in] port A String representing the (numerical) port number of the MarkLogic Server (or load balancer) to connect to
   * \param[in] username The username of the used to connect to MarkLogic Server as
   * \param[in] password The plain text password used to authenticate the user to MarkLogic server with
   * \param[in] usessl A bool representation whether to use SSL (i.e. a HTTPS url) or not
   */
  void configure(const std::string& hostname, const std::string& port, const std::string& username, const std::string& password, const bool usessl = false);

  /**
   * \brief Configures this connection, providing connection override information - host, port, username, password. Defaults to no SSL.
   *
   * TODO do we need this now usessl is defaulted above???
   *
   * \since 8.0.0
   *
   * \param[in] hostname The hostname or IP address of the MarkLogic Server (or load balancer) to connect to
   * \param[in] port A String representing the (numerical) port number of the MarkLogic Server (or load balancer) to connect to
   * \param[in] username The username of the used to connect to MarkLogic Server as
   * \param[in] password The plain text password used to authenticate the user to MarkLogic server with
   */
  void configure(const std::string& hostname, const std::string& port, const std::string& username, const std::string& password);
  /**
   * \brief Sets the name of the database to query/update. Defaults to Documents.
   *
   * \since 8.0.0
   *
   * \param[in] The string name (not internal MarkLogic numeric ID) of the database to query.
   */
  void setDatabaseName(const std::string& db);
  /**
   * \brief Returns the database name we're querying/updating. Defaults to Documents.
   *
   * \since 8.0.0
   *
   * \return A string representation of the database name (not internal MarkLogic database numeric ID)
   */
  std::string getDatabaseName();

  // @}

  // TODO anonymous connection support
  // TODO Kerberos connection support
  // TODO test BASIC and DIGEST and BASIC+DIGEST authentication methods

  // BASIC commands allowing re-use of this connection, perhaps for URLs we don't yet wrap

  /// \name http_raw RAW HTTP commands
  // @{

  /**
   * \brief Performs a HTTP GET Request against MarkLogic Server.
   *
   * Allows mlclient based applications to call any arbitrary REST endpoint on MarkLogic. Just in case we don't yet provide
   * a specific convenience method to invoke it.
   *
   * Performs a RESTful HTTP GET request to an arbitrary URL on MarkLogic Server
   *
   * \since 8.0.0
   *
   * \param[in] pathAndQueryString the path and query string for the entire get request. E.g. /v1/search?options=myoptions
   * \return A unique_ptr for a \link Response \endlink instance. The caller is responsible for destroying the pointer.
   *
   * \exception NoCredentialsException The credentials for the Connection were not accepted by MarkLogic Server,
   * or permission is denied for this request.
   *
   * \note This function is not limited to the REST API, and can be used for the management REST API or any arbitrary MarkLogic
   * web application URL.
   */
  std::unique_ptr<Response> doGet(const std::string& pathAndQuerystring);
  /**
   * \brief Performs a HTTP PUT Request against MarkLogic Server.
   *
   * Allows mlclient based applications to call any arbitrary REST endpoint on MarkLogic. Just in case we don't yet provide
   * a specific convenience method to invoke it.
   *
   * Performs a RESTful HTTP PUT request to an arbitrary URL on MarkLogic Server
   *
   * \since 8.0.0
   *
   * \param[in] pathAndQueryString the path and query string for the entire put request. E.g. /v1/search?options=myoptions
   * \param[in] payload The \link DocumentContent \endlink to send to MarkLogic Server
   * \return A unique_ptr for a \link Response \endlink instance. The caller is responsible for destroying the pointer.
   *
   * \exception NoCredentialsException The credentials for the Connection were not accepted by MarkLogic Server,
   * or permission is denied for this request.
   *
   * \note This function is not limited to the REST API, and can be used for the management REST API or any arbitrary MarkLogic
   * web application URL.
   */
  std::unique_ptr<Response> doPut(const std::string& pathAndQuerystring,const DocumentContent& payload);
  /**
   * \brief Performs a HTTP POST Request against MarkLogic Server.
   *
   * Allows mlclient based applications to call any arbitrary REST endpoint on MarkLogic. Just in case we don't yet provide
   * a specific convenience method to invoke it.
   *
   * Performs a RESTful HTTP POST request to an arbitrary URL on MarkLogic Server
   *
   * \since 8.0.0
   *
   * \param[in] pathAndQueryString the path and query string for the entire post request. E.g. /v1/search?options=myoptions
   * \param[in] payload The \link DocumentContent \endlink to send to MarkLogic Server
   * \return A unique_ptr for a \link Response \endlink instance. The caller is responsible for destroying the pointer.
   *
   * \exception NoCredentialsException The credentials for the Connection were not accepted by MarkLogic Server,
   * or permission is denied for this request.
   *
   * \note This function is not limited to the REST API, and can be used for the management REST API or any arbitrary MarkLogic
   * web application URL.
   */
  std::unique_ptr<Response> doPost(const std::string& pathAndQuerystring,const DocumentContent& payload);

  // TODO multipart payload
  /**
   * \brief Performs a HTTP DELETE Request against MarkLogic Server.
   *
   * Allows mlclient based applications to call any arbitrary REST endpoint on MarkLogic. Just in case we don't yet provide
   * a specific convenience method to invoke it.
   *
   * Performs a HTTP DELETE REST request to an arbitrary URL on MarkLogic Server
   *
   * \since 8.0.0
   *
   * \param[in] pathAndQueryString the path and query string for the entire delete request. E.g. /v1/documents?uri=/my/doc.json
   * \return A unique_ptr for a \link Response \endlink instance. The caller is responsible for destroying the pointer.
   *
   * \exception NoCredentialsException The credentials for the Connection were not accepted by MarkLogic Server,
   * or permission is denied for this request.
   *
   * \note This function is not limited to the REST API, and can be used for the management REST API or any arbitrary MarkLogic
   * web application URL.
   */
  std::unique_ptr<Response> doDelete(const std::string& pathAndQueryString);

  // @}

  // TODO PATCH and other HTTP synonyms

  /// \name wrap_rest Convenience wrapper function calls for common MarkLogic REST API calls.
  // @{

  // Wrapped and supported functions
  /**
   * \brief Retrieves a document from the server, at the given document URI (MarkLogic unique document ID)
   *
   * Performs a GET /v1/documents?uri HTTP call
   *
   * \param[in] uri The URI of the document to fetch from MarkLogic Server
   * \return A unique_ptr for the \link Response \endlink object. The caller is repsonsible for deleting the pointer.
   *
   * \exception NoCredentialsException The credentials for the Connection were not accepted by MarkLogic Server,
   * or permission is denied for this request.
   *
   * \since 8.0.0
   */
  std::unique_ptr<Response> getDocument(const std::string& uri); // TODO add optional call parameters (E.g. fetch properties also)
  /**
   * \brief Saves a document to MarkLogic (either as new or an update), at the given document URI (MarkLogic unique document ID)
   *
   * Performs a call to PUT /v1/documents?ext in order to save a Document to MarkLogic Server
   *
   * \param[in] uri The URI of the document to save to MarkLogic Server
   * \param[in] payload The \link DocumentContent \endlink to send to MarkLogic Server
   * \return A unique_ptr for the \link Response \endlink object. The caller is repsonsible for deleting the pointer.
   *
   * \exception NoCredentialsException The credentials for the Connection were not accepted by MarkLogic Server,
   * or permission is denied for this request.
   *
   * \since 8.0.0
   */
  std::unique_ptr<Response> saveDocument(const std::string& uri,const DocumentContent& payload);
  // TODO save JSON with directory, no uri
  // TODO optional parameters (collection, security, etc.)
  // TODO save XML with and without URI

  // Multi part MIME saveAll function V7+
  //Response saveAllDocuments(const std::string& uris[], const web::json::value& documents[]);
  // TODO XML save all
  // TODO multiple responses when paging in N number of doc transactions

  // CLASHES WITH QTEXT VERSION Response search(const web::json::value& searchQuery,const std::string& options);
  // TODO search with options document (combined query)
  /**
   * \brief Performs a search against the MarkLogic database given the provided search query, text and options
   *
   * Uses a \link SearchDescription \endlink value object to wrap complex search parameters for MarkLogic Server.
   *
   * Performs a POST /v1/search HTTP POST to MarkLogic Server
   *
   * \param[in] desc The SearchDescription defining the search, options, and query string
   * \return A unique_ptr for the \link Response \endlink object. The caller is repsonsible for deleting the pointer.
   *
   * \exception NoCredentialsException The credentials for the Connection were not accepted by MarkLogic Server,
   * or permission is denied for this request.
   *
   * \since 8.0.0
   */
  std::unique_ptr<Response> search(const SearchDescription& desc);


  //std::unique_ptr<Response> search(const web::json::value& searchQuery,const web::json::value& options);
  //std::unique_ptr<Response> search(const web::json::value& searchQuery,const std::string& qtext);
  //std::unique_ptr<Response> search(const web::json::value& searchQuery,const std::string& qtext,const web::json::value& options);
  //std::unique_ptr<Response> search(const SearchDescription& desc);
  // TODO search with XML instead of JSON

  // @}

/*
  // database functions
  // LOW PRIORITY
  void exists();
  void create();
  void destroy();

  // document functions
  // HIGH PRIORITY
  void get(std::string& uri,json::value& options,std::function<void(const Response&)> callback);
  void save(json::value& content,std::string& uri,json::value& properties,std::function<void(const Response&)> callback);
  void delete(std::string& uri,std::function<void(const Response&)> callback);
  // LOW PRIORITY
  void metadata();
  void properties();
  void saveProperties();
  void merge();
  void replaceProperty();
  void saveAll();

  // search functions
  // HIGH PRIORITY
  void search(json::value& query,json::value& options,int start,json::value& searchproperties,std::function<void(const Response&)> callback);
  void structuredSearch(json::value& query,json::value& options,json::value& searchproperties,std::function<void(const Response&)> callback);
  void combined(json::value& structuredQuery,std::string& textQuery,json::value& options,json::value& searchproperties,std::function<void(const Response&)> callback);
  void saveSearchOptions(std::string& name,json::value& options,std::function<void(const Response&)> callback);
  void searchOptions(std::string& name,std::function<void(const Response&)> callback); // gets them
  // MEDIUM PRIORITY
  void values();
  void valuesCombined();
  void saveSearchOptionsCheck();
  // LOW PRIORITY
  void collect();
  void list();
  void keyvalue();
  void searchCollection();
  void suggest();

  // lexicon functions
  // LOW
  void collections();

  // semantic functions
  // LOW
  void saveGraph();
  void mergeGraph();
  void graph();
  void deleteGraph();
  void sparql();

  // configuration functions
  // MEDIUM
  void saveTransform();
  void deleteTransform();
  void getTransform();
  // LOW
  void listTransforms();
  void indexes();

  // transactions
  // MEDIUM
  void begin();
  void commit();
  void rollback();

  // rest extensions from MLJS
  // HIGH
  void version();
  int getVersion(); // returns cached/default version value - no rest request
  // LOW
  void whoami();
  */




private:
  class Impl; // forward declaration - PIMPL idiom
  Impl* mImpl;
};

}

#endif /* defined __MLCLIENT_CONNECTION__ */