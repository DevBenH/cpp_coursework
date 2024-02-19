#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
/* The different types of documents stored in the library */
typedef enum
{
  DOC_NOVEL,
  DOC_COMIC,
  DOC_MAGAZINE
} DocType;

/* Parent class for all types of documents */
class Document
{
public:
  /* return the document type (abstract method) */
  virtual DocType getDocType() = 0;

  /* print the document attributes in a single line on the standard output
   * (abstract method), the format for the various document types is as
   * follows:
   * Novel, title: <title>, author: <author>, year: <year>, quantity: <quantity>
   * Comic, title: <title>, author: <author>, issue: <issue>, year: <year>,
   * quantity: <quantity> Magazine, title: <title>, issue: <issue>, year:
   * <year>, quantity: <quantity>
   */
  virtual void print() = 0;

  /* setters and getters */
  void updateTitle(const std::string &newTitle);
  void updateYear(int newYear);
  void updateQuantity(int newQuantity);
  std::string getTitle();
  int getYear();
  int getQuantity();

  /* Used when someone tries to borrow a document, should return 1 on success
   * and 0 on failure */
  bool borrowDoc();

  /* Used when someone returns a document */
  void returnDoc();

protected:
  /* These need to be protected to be inherited by the child classes */
  std::string _title; // document title
  int _year;          // year of parution
  int _quantity;      // quantity held in the library, should be updated on
                      // borrow (-1) and return (+1)
};

class Novel : public Document
{
public:
  Novel(const std::string &title, const std::string &author, int year, int quantity);
  ~Novel();
  DocType getDocType();
  void print();

  /* getters and setters */
  void updateAuthor(const std::string &newAuthor);
  std::string getAuthor();

private:
  /* In addition to the base Document's attributes, a novel has an author */
  std::string _author;
};

class Comic : public Document
{
public:
  Comic(const std::string &title, const std::string &author, int issue, int year,
        int quantity);
  ~Comic();
  DocType getDocType();
  void print();

  /* getters, setters */
  void updateAuthor(const std::string &newAuthor);
  void updateIssue(int newIssue);
  std::string getAuthor();
  int getIssue();

private:
  /* In addition to the base Document's attributes, a comic has an author as
   * well as an issue number */
  std::string _author;
  int _issue;
};

class Magazine : public Document
{
public:
  Magazine(const std::string &title, int issue, int year, int quantity);
  ~Magazine();
  DocType getDocType();
  void print();

  /* getters, setters */
  void updateIssue(int newIssue);
  int getIssue();

private:
  /* In addition to the base Document's attributes, a magazine has an issue
   * number */
  int _issue;
};

/* One instance of that class represents a library */
class Library
{
public:
  Library();

  /* print the entire library on the standard output, one line per document,
   * in the order they were inserted in the library, using the print()
   * methods implemented by the document objects */
  void print();

  /* Dump the library in a CSV file. The format is:
   * 1 line per file:
   * <document type>,<title>,<author>,<issue>,<year>,<quantity>
   * A field not relevant for a given document type (e.g. issue for novel)
   * is left blank. Return 1 on success, 0 on failure. */
  bool dumpCSV(const std::string &filename);

  /* search for a document in the library, based on the title. We assume that
   * a title identifies uniquely a document in the library, i.e. there cannot
   * be 2 documents with the same title. Returns a pointer to the document if
   * found, NULL otherwise */
  Document *searchDocument(const std::string &title);

  /* Add/delete a document to/from the library, return 1 on success and
   * 0 on failure.  */
  bool addDocument(DocType t, const std::string &title, const std::string &author, int issue,
                   int year, int quantity);
  bool addDocument(std::unique_ptr<Document> d);
  bool delDocument(const std::string &title);

  /* Count the number of document of a given type present in the library */
  int countDocumentOfType(DocType t);

  /* Borrow/return documents, return 1 on success, 0 on failure */
  bool borrowDoc(const std::string &title);
  bool returnDoc(const std::string &title);

private:
  /* Holds all documents in the library */
  // Document *_docs[32 * 1024];

  std::vector<std::unique_ptr<Document>> _docs;
};
