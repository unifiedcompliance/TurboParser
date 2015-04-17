// Copyright (c) 2012-2015 Andre Martins
// All Rights Reserved.
//
// This file is part of TurboParser 2.3.
//
// TurboParser 2.3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// TurboParser 2.3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with TurboParser 2.3.  If not, see <http://www.gnu.org/licenses/>.

#ifndef COREFERENCEDICTIONARY_H_
#define COREFERENCEDICTIONARY_H_

#include "Dictionary.h"
#include "TokenDictionary.h"
#include "DependencyDictionary.h"
#include "SemanticDictionary.h"
#include "SerializationUtils.h"
#include "CoreferenceReader.h"

class Pipe;

class CoreferenceDictionary : public Dictionary {
 public:
  CoreferenceDictionary() {}
  CoreferenceDictionary(Pipe* pipe) : pipe_(pipe) {}
  virtual ~CoreferenceDictionary() { Clear(); }

  virtual void Clear() {
    // Don't clear token_dictionary, since this class does not own it.
    entity_alphabet_.clear();
    constituent_alphabet_.clear();
  }

  virtual void Save(FILE *fs) {
    if (0 > entity_alphabet_.Save(fs)) CHECK(false);
    if (0 > constituent_alphabet_.Save(fs)) CHECK(false);
  }

  void Load(FILE *fs) {
    if (0 > entity_alphabet_.Load(fs)) CHECK(false);
    if (0 > constituent_alphabet_.Load(fs)) CHECK(false);
    entity_alphabet_.BuildNames();
    constituent_alphabet_.BuildNames();
  }

  void AllowGrowth() {
    entity_alphabet_.AllowGrowth();
    constituent_alphabet_.AllowGrowth();
    token_dictionary_->AllowGrowth();
    dependency_dictionary_->AllowGrowth();
    semantic_dictionary_->AllowGrowth();
  }
  void StopGrowth() {
    entity_alphabet_.StopGrowth();
    constituent_alphabet_.StopGrowth();
    token_dictionary_->StopGrowth();
    dependency_dictionary_->StopGrowth();
    semantic_dictionary_->StopGrowth();
  }

  void CreateEntityDictionary(CoreferenceSentenceReader *reader);

  void CreateConstituentDictionary(CoreferenceSentenceReader *reader);

  void BuildEntityNames() {
    entity_alphabet_.BuildNames();
  }

  void BuildConstituentNames() {
    constituent_alphabet_.BuildNames();
  }

  const string &GetEntityName(int tag) const {
    return entity_alphabet_.GetName(tag);
  }

  const string &GetConstituentName(int tag) const {
    return constituent_alphabet_.GetName(tag);
  }

  TokenDictionary *GetTokenDictionary() const { return token_dictionary_; }
  DependencyDictionary *GetDependencyDictionary() const {
    return dependency_dictionary_;
  }
  SemanticDictionary *GetSemanticDictionary() const {
    return semantic_dictionary_;
  }
  void SetTokenDictionary(TokenDictionary *token_dictionary) {
    token_dictionary_ = token_dictionary;
  }
  void SetDependencyDictionary(DependencyDictionary *dependency_dictionary) {
    dependency_dictionary_ = dependency_dictionary;
  }
  void SetSemanticDictionary(SemanticDictionary *semantic_dictionary) {
    semantic_dictionary_ = semantic_dictionary;
  }

  const Alphabet &GetConstituentAlphabet() const {
    return constituent_alphabet_;
  };

  const Alphabet &GetEntityAlphabet() const {
    return entity_alphabet_;
  };

 protected:
  Pipe *pipe_;
  TokenDictionary *token_dictionary_;
  DependencyDictionary *dependency_dictionary_;
  SemanticDictionary *semantic_dictionary_;
  Alphabet entity_alphabet_;
  Alphabet constituent_alphabet_;
  std::set<int> excluded_entity_tags_;
  std::set<int> noun_phrase_tags_;
  std::set<int> proper_noun_tags_;
  std::set<int> pronominal_tags_;
  //Alphabet tag_alphabet_;
};

#endif /* COREFERENCEDICTIONARY_H_ */
