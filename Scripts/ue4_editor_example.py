"""
UE4 编辑器内 Python 脚本示例

此脚本需要在 UE4 编辑器内运行，不能在外部 Python 环境中运行。

运行方法：
1. 在 UE4 编辑器中，打开 Window -> Developer Tools -> Output Log
2. 在 Output Log 的 Cmd 输入框中输入：py "Scripts/ue4_editor_example.py"
3. 或者在编辑器菜单中选择 File -> Execute Python Script，然后选择此文件
"""

import unreal

def main():
    """主函数"""
    unreal.log("=" * 60)
    unreal.log("UE4 编辑器 Python 脚本示例")
    unreal.log("=" * 60)
    
    # 获取编辑器资产库
    asset_lib = unreal.EditorAssetLibrary()
    
    # 列出 Content 目录下的所有资产
    unreal.log("\n正在列出 /Game/ 目录下的资产...")
    assets = asset_lib.list_assets("/Game/", recursive=True, include_folder=False)
    
    unreal.log(f"\n找到 {len(assets)} 个资产：")
    for asset_path in assets[:10]:  # 只显示前 10 个
        unreal.log(f"  - {asset_path}")
    
    if len(assets) > 10:
        unreal.log(f"  ... 还有 {len(assets) - 10} 个资产")
    
    # 获取当前关卡
    level_lib = unreal.EditorLevelLibrary()
    current_level = level_lib.get_editor_world()
    
    if current_level:
        unreal.log(f"\n当前关卡：{current_level.get_name()}")
        
        # 获取关卡中的所有 Actor
        actors = level_lib.get_all_level_actors()
        unreal.log(f"关卡中有 {len(actors)} 个 Actor")
    
    unreal.log("\n" + "=" * 60)
    unreal.log("脚本执行完成！")
    unreal.log("=" * 60)

if __name__ == "__main__":
    main()
