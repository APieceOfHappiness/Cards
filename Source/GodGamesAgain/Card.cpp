// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"


namespace Gen {
	Card::Card() {}

	FString Card::GetDescription()
	{
		return FString(TEXT("Template"));
	}

	Card::~Card() {}

	Warrior::Warrior(int Hp, int Damage) {
		this->StartHp = Hp;
		this->StartDamage = Damage;
		this->CurHp = this->StartHp;
		this->CurDamage = this->StartDamage;
	}

	int Warrior::GetState() const {
		return this->State;
	}
	int Warrior::GetHp() const {
		return this->CurHp;
	}
	int Warrior::GetDamage() const {
		return this->CurDamage;
	}
	void Warrior::Recreate() {
		this->CurHp = this->StartHp;
		this->CurDamage = this->StartDamage;
		this->State = State::ALIVE;
	}
	void Warrior::Attack(int Damage) {
		this->CurHp -= Damage;
		if (this->CurHp <= 0) {
			this->State = State::DEAD;
		}
	}
	void Warrior::Heal(int Hp) {
		this->CurHp += Hp;
		if (this->CurHp > 0) {
			this->State = State::ALIVE;
		}
	}
	void Warrior::Slacken(float WeakCoef) {
		this->CurDamage /= WeakCoef;
	}
	void Warrior::Boost(float BoostCoef) {
		this->CurDamage *= BoostCoef;
	}

	FString Warrior::GetDescription()
	{
		return FString::Printf(TEXT("HP\n|%d|\nDamage\n|%d|"), this->GetHp(), this->GetDamage());
	}

	WarriorDeck::WarriorDeck()
	{
		for(int Hp = 100; Hp <= 2000; Hp += 100) {
			this->Hp.push_back(Hp);
		}
		for (int Damage = 100; Damage <= 2000; Damage += 100) {
			this->Damage.push_back(Damage);
		}
	}

	void WarriorDeck::SetParams(const std::vector<std::vector<float>>& Params)
	{
		this->Hp.clear();
		this->Damage.clear();
		for (int HpIdx = 0; HpIdx <= Params[0].size(); ++HpIdx) {
			this->Hp.push_back(Params[0][HpIdx]);
		}
		for (int DamageIdx = 0; DamageIdx <= Params[1].size(); ++DamageIdx) {
			this->Damage.push_back(Params[1][DamageIdx]);
		}
	}

	std::vector<std::vector<float>> WarriorDeck::GetParams()
	{
		std::vector<std::vector<float>> Params(2, std::vector<float>());
		for (int HpIdx = 0; HpIdx < this->Hp.size(); ++HpIdx) {
			Params[0].push_back(this->Hp[HpIdx]);
		}
		for (int DamageIdx = 0; DamageIdx < this->Hp.size(); ++DamageIdx) {
			Params[1].push_back(this->Damage[DamageIdx]);
		}
		return Params;
	}

	//WarriorDeck::WarriorDeck(int HpMin, int HpMax, int HpStep, int DamageMin, int DamageMax, int DamageStep)
	//{
	//	this->HpMin = HpMin;
	//	this->HpMax = HpMax;
	//	this->HpStep = HpStep;
	//	this->DamageMin = DamageMin;
	//	this->DamageMax = DamageMax;
	//	this->DamageStep = DamageStep;
	//}

	Warrior* WarriorDeck::GenerateCard()
	{
		int NewHpIdx = FMath::RandRange(0, this->Hp.size() - 1);
		int NewDamageIdx = FMath::RandRange(0, this->Damage.size() - 1);
		return new Warrior(this->Hp[NewHpIdx], this->Damage[NewDamageIdx]);
	}

	CardOfChange::CardOfChange() : CardOfChange::CardOfChange(ChangeName::BASE) {};

	CardOfChange::CardOfChange(int Type) : Type(Type) {};

	int CardOfChange::IsPositive() const
	{
		return this->Positive;
	}

	int CardOfChange::GetRelativeTarget() const
	{
		return this->RelativeTarget;
	}

	int CardOfChange::GetType() const
	{
		return this->Type;
	}

	HealingCard::HealingCard() : CardOfChange(ChangeName::HEAL) {};

	HealingCard::HealingCard(int RelativeTarget, int HpIncrease) : HealingCard::HealingCard()
	{
		this->RelativeTarget = RelativeTarget;
		this->HpIncrease = HpIncrease;
	}

	void HealingCard::Apply(std::vector<Warrior*>& Warriors)
	{
		for (auto& OneWarrior : Warriors) {
			OneWarrior->Heal(this->HpIncrease);
		}
	}

	FString HealingCard::GetDescription()
	{
		if (this->GetRelativeTarget() == RTarget::SINGLE) {
			return FString::Printf(TEXT("Target\n|SINGLE|\nHpIncr\n|%d|"), this->HpIncrease);
		} else if (this->GetRelativeTarget() == RTarget::DIAG) {
			return FString::Printf(TEXT("Target\n|DIAG|\nHpIncr\n|%d|"), this->HpIncrease);
		}
		return FString::Printf(TEXT("ERROR"));
	}

	PoisonCard::PoisonCard() : CardOfChange(ChangeName::POSION) {};

	PoisonCard::PoisonCard(int RelativeTarget, int HpDecrease) : PoisonCard::PoisonCard()
	{
		this->RelativeTarget = RelativeTarget;
		this->HpDecrease = HpDecrease;
	}
	void PoisonCard::Apply(std::vector<Warrior*>& Warriors)
	{
		for (auto& OneWarrior : Warriors) {
			OneWarrior->Attack(this->HpDecrease);
		}
	}
	FString PoisonCard::GetDescription()
	{
		if (this->GetRelativeTarget() == RTarget::SINGLE) {
			return FString::Printf(TEXT("Target\n|SINGLE|\nHpDecr\n|%d|"), this->HpDecrease);
		}
		else if (this->GetRelativeTarget() == RTarget::DIAG) {
			return FString::Printf(TEXT("Target\n|DIAG|\nHpDecr\n|%d|"), this->HpDecrease);
		}
		return FString::Printf(TEXT("ERROR"));
	}

	WeakeningCard::WeakeningCard() : CardOfChange(ChangeName::WEAK) {};

	WeakeningCard::WeakeningCard(int RelativeTarget, float WeakCoef) : WeakeningCard::WeakeningCard()
	{
		this->RelativeTarget = RelativeTarget;
		this->WeakCoef = WeakCoef;
	}
	void WeakeningCard::Apply(std::vector<Warrior*>& Warriors)
	{
		for (auto& OneWarrior : Warriors) {
			OneWarrior->Slacken(this->WeakCoef);
		}
	}

	FString WeakeningCard::GetDescription()
	{
		if (this->GetRelativeTarget() == RTarget::SINGLE) {
			return FString::Printf(TEXT("Target\n|SINGLE|\nWeakCoef\n|%.1f|"), this->WeakCoef);
		}
		else if (this->GetRelativeTarget() == RTarget::DIAG) {
			return FString::Printf(TEXT("Target\n|DIAG|\nWeakCoef\n|%.1f|"), this->WeakCoef);
		}
		return FString::Printf(TEXT("ERROR"));
	}

	BoostingCard::BoostingCard() : CardOfChange(ChangeName::BOOST) {};

	BoostingCard::BoostingCard(int RelativeTarget, float BoostCoef) : BoostingCard::BoostingCard()
	{
		this->RelativeTarget = RelativeTarget;
		this->BoostCoef = BoostCoef;
	}
	void BoostingCard::Apply(std::vector<Warrior*>& Warriors)
	{
		for (auto& OneWarrior : Warriors) {
			OneWarrior->Boost(this->BoostCoef);
		}
	}

	FString BoostingCard::GetDescription()
	{
		if (this->GetRelativeTarget() == RTarget::SINGLE) {
			return FString::Printf(TEXT("Target\n|SINGLE|\nBoostCoef\n|%.1f|"), this->BoostCoef);
		}
		else if (this->GetRelativeTarget() == RTarget::DIAG) {
			return FString::Printf(TEXT("Target\n|DIAG|\nBoostCoef\n|%.1f|"), this->BoostCoef);
		}
		return FString::Printf(TEXT("ERROR"));
	}

	EmptyCard::EmptyCard() : CardOfChange(ChangeName::EMPTY) {};
	void EmptyCard::Apply(std::vector<Warrior*>& Warriors) {}

	FString EmptyCard::GetDescription()
	{
		return FString::Printf(TEXT("Empty\nCard"));
	};

	ChangeDeck::ChangeDeck() 
	{
		this->CntClasses = 5;
		this->Params.resize(this->CntClasses, std::vector<float>());
		for (float HpIncrease = 100; HpIncrease <= 2000; HpIncrease += 100) {
			this->Params[0].push_back(HpIncrease);
		}
		for (float HpDecrease = 100; HpDecrease <= 2000; HpDecrease += 100) {
			this->Params[1].push_back(HpDecrease);
		}
		for (float WeakCoef = 1.1f; WeakCoef <= 3; WeakCoef += 0.1f) {
			this->Params[2].push_back(WeakCoef);
		}
		for (float BoostCoef = 1.1f; BoostCoef <= 3; BoostCoef += 0.1f) {
			this->Params[3].push_back(BoostCoef);
		}
	};

	void ChangeDeck::SetParams(const std::vector<std::vector<float>>& Params)
	{
		this->Params.clear();
		this->Params.resize(Params.size(), std::vector<float>());
		for (int ClassIdx = 0; ClassIdx < this->CntClasses; ++ClassIdx) {
			for (int j = 0; j < Params[ClassIdx].size(); ++j) {
				this->Params[ClassIdx].push_back(Params[ClassIdx][j]);
			}
		}
	}

	std::vector<std::vector<float>> ChangeDeck::GetParams() const
	{
		//std::vector<std::vector<float>> Params(this->Params.size(), std::vector<float>());
		//for (int ClassIdx = 0; ClassIdx < this->CntClasses; ++ClassIdx) {
		//	for (int j = 0; j < this->Params[ClassIdx].size(); ++j) {
		//		Params[ClassIdx].push_back(this->Params[ClassIdx][j]);
		//	}
		//}
		return this->Params;
	}

	//void ChangeDeck::ÑhangeClassicParams(const std::vector<std::vector<float>>& NewParams)
	//{
	//	for (int i = 0; i < this->Params.size(); ++i) {
	//		for (int j = 0; j < this->Params[i].size(); ++j) {
	//			this->Params[i][j] = NewParams[i][j];
	//		}
	//	}
	//}

	CardOfChange* ChangeDeck::GenerateCard()
	{
		int RandomClass = FMath::RandRange(0, this->CntClasses - 1);
		if (RandomClass == 0) {
			int HpIncreaseIdx = FMath::RandRange(0, this->Params[0].size() - 1);
			return new HealingCard(FMath::RandRange(0, 1), this->Params[0][HpIncreaseIdx]);
		}
		else if (RandomClass == 1) {
			int HpDecreaseIdx = FMath::RandRange(0, this->Params[1].size() - 1);
			return new PoisonCard(FMath::RandRange(0, 1), this->Params[1][HpDecreaseIdx]);
		}
		else if (RandomClass == 2) {
			int WeakCoef = FMath::RandRange(0, this->Params[2].size() - 1);
			return new WeakeningCard(FMath::RandRange(0, 1), this->Params[2][WeakCoef]);
		}
		else if (RandomClass == 3) {
			int BoostCoef = FMath::RandRange(0, this->Params[3].size() - 1);
			return new BoostingCard(FMath::RandRange(0, 1), this->Params[3][BoostCoef]);
		}
		else {
			return new EmptyCard();
		}
	}
};
