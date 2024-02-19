#include "library.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <string>
#include <vector>
#include <iostream> //extra imports added
#include <fstream>
#include <memory>
#include <sstream>
#include <algorithm>

void Document::updateTitle(const std::string &newTitle)
{
  // assign the title a new title
  _title.assign(newTitle);
}

void Document::updateYear(int newYear) { _year = newYear; }
void Document::updateQuantity(int newQuantity) { _quantity = newQuantity; }
std::string Document::getTitle() { return _title; }
int Document::getYear() { return _year; }
int Document::getQuantity() { return _quantity; }

bool Document::borrowDoc()
{
  if (_quantity > 0)
  {
    _quantity--;
    return true;
  }
  return false;
}

void Document::returnDoc() { _quantity++; }

// create a new novel
Novel::Novel(const std::string &title, const std::string &author, int year, int quantity)
{
  _title.assign(title);
  _author.assign(author);

  _year = year;
  _quantity = quantity;
}

Novel::~Novel()
{
}

DocType Novel::getDocType() { return DOC_NOVEL; }

void Novel::print()
{
  // use the std library for cpp to output a string
  std::cout << "Novel, title: " << _title << ", author: " << _author << ", year: " << _year << ", quantity: " << _quantity << "\n";
}

void Novel::updateAuthor(const std::string &newAuthor)
{
  _author.assign(newAuthor);
}

std::string Novel::getAuthor() { return _author; }

// create a new comic
Comic::Comic(const std::string &title, const std::string &author, int issue, int year,
             int quantity)
{
  _title.assign(title);
  _author.assign(author);
  _year = year;
  _quantity = quantity;
  _issue = issue;
}

Comic::~Comic()
{
}

DocType Comic::getDocType() { return DOC_COMIC; }

void Comic::print()
{
  // output a string of the comic attributes
  std::cout << "Comic, title: " << _title << ", author: " << _author << ", issue: " << _issue << ", year: " << _year << ", quantity: " << _quantity << "\n";
}

void Comic::updateAuthor(const std::string &newAuthor)
{
  _author.assign(newAuthor);
}

void Comic::updateIssue(int newIssue) { _issue = newIssue; }
std::string Comic::getAuthor() { return _author; }
int Comic::getIssue() { return _issue; }

Magazine::Magazine(const std::string &title, int issue, int year, int quantity)
{
  _title.assign(title);

  _year = year;
  _quantity = quantity;
  _issue = issue;
}

Magazine::~Magazine() {}

DocType Magazine::getDocType() { return DOC_MAGAZINE; }

void Magazine::print()
{
  // output a string of the magazine values
  std::cout << "Magazine, title: " << _title << ", issue: " << _issue << ", year: " << _year << ", quantity: " << _quantity << "\n";
}

void Magazine::updateIssue(int newIssue) { _issue = newIssue; }
int Magazine::getIssue() { return _issue; }

Library::Library(){};

bool Library::addDocument(DocType t, const std::string &title, const std::string &author,
                          int issue, int year, int quantity)
{

  // create a unique pointer
  std::unique_ptr<Document> d;

  switch (t)
  {
  case DOC_NOVEL:
  {
    // assign the unqiue pointer
    d = std::make_unique<Novel>(title, author, year, quantity);
    break;
  }

  case DOC_COMIC:
  {
    d = std::make_unique<Comic>(title, author, issue, year, quantity);
    break;
  }

  case DOC_MAGAZINE:
  {
    d = std::make_unique<Magazine>(title, issue, year, quantity);
    break;
  }

  default:
    return false;
  }
  return addDocument(move(d));
}

bool Library::addDocument(std::unique_ptr<Document> d)
{

  // loop through the vector of unqiue pointers
  for (auto &i : _docs)
  {
    if (i->getTitle() == d->getTitle())
      return false;
  }
  // add the document to the vector
  _docs.push_back(move(d));
  return true;
}

bool Library::delDocument(const std::string &title)
{
  // loop through the vector

  for (auto &i : _docs)
  {
    // if the titles match
    if (i->getTitle() == title)
    {
      _docs.erase(std::find(_docs.begin(), _docs.end(), i));
      return true;
    }
  }

  return false;
}

int Library::countDocumentOfType(DocType t)
{
  int res{0};
  // loop through the vector
  for (auto &i : _docs)
  {
    if (i->getDocType() == t)
      res++;
  }
  return res;
}

Document *Library::searchDocument(const std::string &title)
{
  for (auto &i : _docs)
  {
    if ((i->getTitle() == title))
      // return the value at the pointer
      return i.get();
  }
  return NULL;
}

void Library::print()
{
  for (auto &i : _docs)
  {
    // print the value of the pointer in the vector
    i->print();
  }
}

bool Library::borrowDoc(const std::string &title)
{
  // search the vector for documents with the name title
  Document *d = searchDocument(title);
  // if a document was found
  if (d)
  {
    return d->borrowDoc();
  }
  return false;
}

bool Library::returnDoc(const std::string &title)
{
  Document *d = searchDocument(title);
  if (d)
  {
    d->returnDoc();
    return true;
  }
  return false;
}

bool Library::dumpCSV(const std::string &filename)
{
  char line[128];
  // create an output file stream
  std::ofstream infile(filename);

  if (!infile)
    return false;

  for (auto &i : _docs)
  {
    Document *d = i.get();
    switch (d->getDocType())
    {
    case DOC_NOVEL:
    {
      // dynamically cast the unique pointer
      Novel *n = dynamic_cast<Novel *>(d);
      std::ostringstream buffer;
      // write to the file using an output stream
      buffer << "novel," << n->getTitle() << "," << n->getAuthor() << ",," << n->getYear() << "," << n->getQuantity() << "\n";
      infile << buffer.str();
      break;
    }

    case DOC_COMIC:
    {
      Comic *c = dynamic_cast<Comic *>(d);
      std::ostringstream buffer;
      buffer << "comic," << c->getTitle() << "," << c->getAuthor() << "," << c->getIssue() << "," << c->getYear() << "," << c->getQuantity() << "\n";
      infile << buffer.str();
      break;
    }

    case DOC_MAGAZINE:
    {
      Magazine *m = dynamic_cast<Magazine *>(d);
      std::ostringstream buffer;
      buffer << "magazine," << m->getTitle() << ",," << m->getIssue() << "," << m->getYear() << "," << m->getQuantity() << "\n";
      infile << buffer.str();
      break;
    }

    default:
      return false;
    }
  }
  return true;
}
