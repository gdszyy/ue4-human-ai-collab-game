# 《回声炼金师》项目结构说明

## 项目概述

本项目是基于 **UE5 5.7** 的游戏开发项目，采用人机协同开发模式。

## 目录结构

```
ue5-human-ai-collab-game/
├── EchoAlchemist.uproject          # UE5 项目文件
├── Config/                          # 项目配置文件
│   ├── DefaultEngine.ini           # 引擎配置
│   ├── DefaultEditor.ini           # 编辑器配置
│   └── DefaultGame.ini             # 游戏配置
├── Content/                         # 游戏资产目录
│   ├── Blueprints/                 # 蓝图资产
│   ├── Maps/                       # 关卡地图
│   ├── Characters/                 # 角色资产
│   ├── Systems/                    # 游戏系统
│   ├── Materials/                  # 材质资产
│   ├── Textures/                   # 纹理资产
│   └── Audio/                      # 音频资产
├── Source/                          # C++ 源代码
│   ├── EchoAlchemist/              # 游戏模块
│   │   ├── Public/                 # 公共头文件
│   │   └── Private/                # 私有实现文件
│   ├── EchoAlchemist.Target.cs     # 游戏构建目标
│   └── EchoAlchemistEditor.Target.cs # 编辑器构建目标
├── Scripts/                         # Python 脚本
│   ├── test_remote_control.py      # Remote Control API 测试脚本
│   └── ue5_editor_example.py       # UE5 编辑器内脚本示例
├── Docs/                            # 项目文档
│   ├── PROJECT_STRUCTURE.md        # 本文件
│   └── LOCAL_SETUP_GUIDE.md        # 本地环境配置指南
├── .gitignore                       # Git 忽略规则
├── .gitattributes                   # Git LFS 配置
└── README.md                        # 项目说明
```

## 核心配置

### 已启用的插件

1. **RemoteControl**: 远程控制 API，支持通过 HTTP/WebSocket 远程操作 UE5
2. **PythonScriptPlugin**: Python 脚本支持，用于编辑器自动化
3. **EditorScriptingUtilities**: 编辑器脚本工具

### Remote Control API 配置

在 `Config/DefaultEngine.ini` 中配置：

```ini
[/Script/RemoteControl.RemoteControlSettings]
bEnableRemoteControl=True
RemoteControlWebInterfacePort=30010
bAutoStartWebServer=True
bAutoStartWebSocketServer=True
```

- **默认端口**: 30010
- **自动启动**: 编辑器打开时自动启动 Web Server

## 内容组织规范

### Blueprints 目录

- `Core/`: 核心游戏逻辑蓝图
- `UI/`: 用户界面蓝图
- `Gameplay/`: 游戏玩法蓝图

### Maps 目录

- `DefaultLevel`: 默认关卡
- `TestMaps/`: 测试关卡

### Characters 目录

- `Player/`: 玩家角色
- `Enemies/`: 敌人角色
- `NPCs/`: NPC 角色

### Systems 目录

- `AlchemySystem/`: 炼金系统
- `BallPhysics/`: 弹珠物理系统
- `BlockSystem/`: 砖块系统

## C++ 代码规范

### 命名约定

- **类名**: 使用 `A` 前缀（Actor）或 `U` 前缀（UObject）
- **文件名**: 与类名一致
- **变量名**: 使用 `b` 前缀（bool）、`f` 前缀（float）等

### 模块结构

- **Public/**: 对外暴露的头文件
- **Private/**: 内部实现文件

## Git 工作流

### 提交规范

使用 Conventional Commits 规范：

```
feat: 新功能
fix: 修复 bug
docs: 文档更新
style: 代码格式调整
refactor: 重构
test: 测试相关
chore: 构建/工具相关
```

### Git LFS

大文件类型已配置 Git LFS 跟踪：
- `.uasset`, `.umap`: UE5 资产文件
- `.fbx`: 3D 模型
- `.png`, `.jpg`: 图片
- `.wav`, `.mp3`: 音频

## 人机协同工作流

### Manus → 本地

1. Manus 在沙盒中修改项目文件
2. Manus 推送到 Git 仓库
3. 本地执行 `git pull` 拉取更新
4. UE5 编辑器自动检测并重新加载

### 本地 → Manus

1. 在本地 UE5 中进行开发
2. 提交并推送到 Git 仓库
3. Manus 拉取更新以了解项目状态

## 相关文档

- [本地环境配置指南](LOCAL_SETUP_GUIDE.md)
- [人机协同开发规范](../../ue5-human-ai-collab/docs/collaboration_specification.md)
- [工作流规范](../../ue5-human-ai-collab/docs/workflow_specification.md)
