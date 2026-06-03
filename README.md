# CoreGAS Plugin

A modular Gameplay Ability System plugin for Unreal Engine 5, designed as a drop-in foundation for third-person action combat games. Built in C++.

> 🎮 Used in [ProjectX](https://www.youtube.com/watch?v=EhdeD4uLL0I) — a third-person action game built entirely on this plugin.

---

## Features

- **Ability System** — GameplayTag-based execution conditions, count-based tag stacking, custom Attribute System
- **Effect System** — Instant, duration, and infinite effect types with support for attack speed, lifesteal, and critical damage calculations
- **Targeting System** — Projectile and targeted abilities, combo chains, and CC ability support
- **Multiplayer Ready** — Full replication support across all major gameplay systems
- **AI Integration** — Compatible with custom AI decision-making and combat state management

---

## Requirements

- Unreal Engine 5.7
- C++ project

---

## Setup

### 1. Add the plugin

Clone or copy the plugin into your project's `Plugins/` folder:

```
YourProject/
└── Plugins/
    └── CoreGAS/
```

### 2. Enable the plugin

In your `.uproject` file, add:

```json
{
  "Name": "CoreGAS",
  "Enabled": true
}
```

### 3. Configure collision profiles

Add the following to your project's `Config/DefaultEngine.ini` under `[/Script/Engine.CollisionProfile]`:

```ini
[/Script/Engine.CollisionProfile]
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel1,DefaultResponse=ECR_Ignore,bTraceType=False,bStaticObject=False,Name="Hero")
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Ignore,bTraceType=False,bStaticObject=False,Name="Enemy")
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel3,DefaultResponse=ECR_Ignore,bTraceType=True,bStaticObject=False,Name="Damage")
+Profiles=(Name="Hero",CollisionEnabled=QueryOnly,ObjectTypeName="Hero",CustomResponses=((Channel="Damage",Response=ECR_Block),(Channel="Visibility",Response=ECR_Block)))
+Profiles=(Name="Enemy",CollisionEnabled=QueryOnly,ObjectTypeName="Enemy",CustomResponses=((Channel="Damage",Response=ECR_Block),(Channel="Visibility",Response=ECR_Block)))
```

---

## Project Structure

```
Source/CoreGAS/
├── Abilities/        # Base ability classes
├── Attributes/       # Attribute sets
├── Effects/          # Gameplay effect definitions
└── Targeting/        # Targeting system
```

---

## License

This project is currently unlicensed. All rights reserved.
