"""
测试 UE4 Remote Control API 连接

使用方法：
1. 确保 UE4 编辑器已打开项目
2. 确保 Remote Control 插件已启用
3. 运行此脚本：python test_remote_control.py

注意：此脚本需要在能够访问 UE4 编辑器的网络环境中运行
"""

import requests
import json

# Remote Control API 默认端口
API_BASE_URL = "http://localhost:30010"

def test_connection():
    """测试 Remote Control API 连接"""
    try:
        response = requests.get(f"{API_BASE_URL}/remote/info", timeout=5)
        if response.status_code == 200:
            print("✅ Remote Control API 连接成功！")
            print(f"可用的 HTTP 路由：")
            data = response.json()
            for route in data.get("HttpRoutes", []):
                print(f"  - {route['Verb']} {route['Path']}: {route['Description']}")
            return True
        else:
            print(f"❌ 连接失败，状态码：{response.status_code}")
            return False
    except requests.exceptions.ConnectionError:
        print("❌ 无法连接到 Remote Control API")
        print("请确保：")
        print("  1. UE4 编辑器已打开项目")
        print("  2. Remote Control 插件已启用")
        print("  3. Remote Control Web Server 已启动")
        return False
    except Exception as e:
        print(f"❌ 发生错误：{e}")
        return False

def test_search_assets():
    """测试搜索资产功能"""
    try:
        response = requests.put(
            f"{API_BASE_URL}/remote/search/assets",
            json={
                "Query": "*",
                "Limit": 10
            },
            timeout=5
        )
        if response.status_code == 200:
            print("\n✅ 资产搜索测试成功！")
            data = response.json()
            print(f"找到 {len(data.get('Assets', []))} 个资产")
            return True
        else:
            print(f"\n❌ 资产搜索失败，状态码：{response.status_code}")
            return False
    except Exception as e:
        print(f"\n❌ 资产搜索发生错误：{e}")
        return False

if __name__ == "__main__":
    print("=" * 60)
    print("UE4 Remote Control API 连接测试")
    print("=" * 60)
    
    if test_connection():
        test_search_assets()
    
    print("\n" + "=" * 60)
