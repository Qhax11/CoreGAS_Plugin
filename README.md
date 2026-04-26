# CoreGAS Plugin

## Setup

After adding the plugin to your project, add the following to your project's `Config/DefaultEngine.ini` under `[/Script/Engine.CollisionProfile]`:

```ini
[/Script/Engine.CollisionProfile]
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel1,DefaultResponse=ECR_Ignore,bTraceType=False,bStaticObject=False,Name="Hero")
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Ignore,bTraceType=False,bStaticObject=False,Name="Enemy")
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel3,DefaultResponse=ECR_Ignore,bTraceType=True,bStaticObject=False,Name="Damage")
+Profiles=(Name="Hero",CollisionEnabled=QueryOnly,ObjectTypeName="Hero",CustomResponses=((Channel="Damage",Response=ECR_Block),(Channel="Visibility",Response=ECR_Block)))
+Profiles=(Name="Enemy",CollisionEnabled=QueryOnly,ObjectTypeName="Enemy",CustomResponses=((Channel="Damage",Response=ECR_Block),(Channel="Visibility",Response=ECR_Block)))
```
