// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>
#include "Math/RandomStream.h"
#include "CoreMinimal.h"

namespace Gen {
	namespace State {
		enum { DEAD, ALIVE };
	};

	namespace RTarget {
		enum { SINGLE, DIAG };
	};

	namespace ChangeName {
		enum { BASE, HEAL, POSION, WEAK, BOOST, EMPTY };
	};

	class GODGAMESAGAIN_API Card
	{
	public:
		Card();
		virtual FString GetDescription();
		~Card();
	};

	class GODGAMESAGAIN_API Warrior : public Card {
		int StartHp = 500;
		int StartDamage = 100;
		int CurHp = StartHp;
		int CurDamage = StartDamage;
		int State = State::ALIVE;

	public:
		Warrior() = default;
		Warrior(int Hp, int Damage);
		int GetState() const;
		int GetHp() const;
		int GetDamage() const;
		void Recreate();
		void Attack(int Damage);
		void Heal(int Hp);
		void Slacken(float WeakCoef);
		void Boost(float BoostCoef);
		FString GetDescription() override;

		~Warrior() = default;
	};

	class GODGAMESAGAIN_API WarriorDeck {
		std::vector<float> Hp;
		std::vector<float> Damage;
		//int HpStep = 100;
		//int DamageStep = 100;
		//int HpMin = 100;
		//int HpMax = 1000;
		//int DamageMin = 100;
		//int DamageMax = 1000;
	public:
		WarriorDeck();
		
		void SetParams(const std::vector<std::vector<float>>& Params);
		std::vector<std::vector<float>> GetParams();

		Warrior* GenerateCard();
		~WarriorDeck() = default;
	};

	class GODGAMESAGAIN_API CardOfChange : public Card {
		int Positive = 1;
		int RelativeTarget = RTarget::SINGLE;
		int Type = ChangeName::BASE;

	public:
		CardOfChange();
		CardOfChange(int Type);
		int IsPositive() const;
		int GetRelativeTarget() const;
		int GetType() const;
		virtual void Apply(std::vector<Warrior*>& warriors) {};
	};

	class GODGAMESAGAIN_API HealingCard : public CardOfChange {
		int Positive = 1;
		int RelativeTarget = RTarget::SINGLE;
		int HpIncrease = 500;
		int Type = ChangeName::HEAL;

	public:
		HealingCard();
		HealingCard(int RelativeTarget, int HpIncrease);
		void Apply(std::vector<Warrior*>& Warriors) override;
		FString GetDescription() override;
	};

	class GODGAMESAGAIN_API PoisonCard : public CardOfChange {
		int Positive = 0;
		int RelativeTarget = RTarget::SINGLE;
		int HpDecrease = 500;
		int Type = ChangeName::POSION;

	public:
		PoisonCard();
		PoisonCard(int RelativeTarget, int HpDecrease);
		void Apply(std::vector<Warrior*>& Warriors) override;
		FString GetDescription() override;
	};

	class GODGAMESAGAIN_API WeakeningCard : public CardOfChange {
		int Positive = 0;
		int RelativeTarget = RTarget::SINGLE;
		float WeakCoef = 1.5f;
		int Type = ChangeName::WEAK;

	public:
		WeakeningCard();
		WeakeningCard(int RelativeTarget, float WeakCoef);
		void Apply(std::vector<Warrior*>& Warriors) override;
		FString GetDescription() override;
	};

	class GODGAMESAGAIN_API BoostingCard : public CardOfChange {
		int Positive = 1;
		int RelativeTarget = RTarget::SINGLE;
		float BoostCoef = 1.5f;
		int Type = ChangeName::BOOST;

	public:
		BoostingCard();
		BoostingCard(int RelativeTarget, float BoostCoef);
		void Apply(std::vector<Warrior*>& Warriors) override;
		FString GetDescription() override;
	};

	class GODGAMESAGAIN_API EmptyCard : public CardOfChange {
		int Positive = 1;
		int RelativeTarget = RTarget::SINGLE;
		int Type = ChangeName::EMPTY;

	public:
		EmptyCard();
		void Apply(std::vector<Warrior*>& Warriors) override;
		FString GetDescription() override;
	};

	class GODGAMESAGAIN_API ChangeDeck {
		std::vector<std::vector<float>> Params;
		int CntClasses = 5;
	public:
		ChangeDeck();

		void SetParams(const std::vector<std::vector<float>>& Params);
		std::vector<std::vector<float>> GetParams() const;
		CardOfChange* GenerateCard();
	};
}

