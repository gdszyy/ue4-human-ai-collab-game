#!/usr/bin/env python3
"""
生成临时测试精灵
用于快速测试VOI-11怪兽生成系统

这些精灵是简单的纯色矩形，用于验证系统功能
正式版本需要替换为美术设计的精灵
"""

from PIL import Image, ImageDraw, ImageFont
import os

# 输出目录
SKELETON_DIR = "TempAssets/Sprites/Skeletons"
PART_DIR = "TempAssets/Sprites/Parts"

# 精灵尺寸
SPRITE_SIZE = (256, 256)

# 栖息地配置
HABITATS = [
    ("Forest", (34, 139, 34)),      # 森林绿
    ("Desert", (210, 180, 140)),    # 沙漠黄
    ("Tundra", (176, 224, 230)),    # 冰原蓝
    ("Swamp", (85, 107, 47)),       # 沼泽深绿
    ("Mountain", (139, 137, 137)),  # 山地灰
    ("Ocean", (0, 105, 148)),       # 海洋深蓝
]

# 体型配置
SIZE_CLASSES = [
    ("Tiny", 0.4),
    ("Small", 0.6),
    ("Medium", 0.8),
    ("Large", 1.0),
    ("Giant", 1.0),
]

# 部件配置
PARTS = [
    ("FireWings", (255, 69, 0)),      # 火焰橙红
    ("FireTail", (255, 140, 0)),      # 火焰橙
    ("RockArmor", (105, 105, 105)),   # 岩石灰
    ("IceHorns", (135, 206, 250)),    # 冰霜蓝
    ("PoisonSpikes", (124, 252, 0)),  # 毒刺绿
    ("LightningBolt", (255, 255, 0)), # 闪电黄
    ("ShadowCloak", (47, 79, 79)),    # 暗影灰
    ("HealingAura", (255, 182, 193)), # 治愈粉
]


def create_skeleton_sprite(habitat_name, size_class_name, color, scale):
    """创建骨架精灵"""
    img = Image.new('RGBA', SPRITE_SIZE, (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 计算实际大小
    base_size = int(200 * scale)
    offset_x = (SPRITE_SIZE[0] - base_size) // 2
    offset_y = (SPRITE_SIZE[1] - base_size) // 2
    
    # 绘制身体（椭圆）
    body_bbox = [
        offset_x,
        offset_y + base_size // 4,
        offset_x + base_size,
        offset_y + base_size
    ]
    draw.ellipse(body_bbox, fill=color)
    
    # 绘制头部（圆形）
    head_size = base_size // 3
    head_bbox = [
        offset_x + base_size // 2 - head_size // 2,
        offset_y,
        offset_x + base_size // 2 + head_size // 2,
        offset_y + head_size
    ]
    draw.ellipse(head_bbox, fill=color)
    
    # 绘制眼睛
    eye_size = head_size // 5
    eye_y = offset_y + head_size // 3
    # 左眼
    draw.ellipse([
        offset_x + base_size // 2 - head_size // 4 - eye_size // 2,
        eye_y,
        offset_x + base_size // 2 - head_size // 4 + eye_size // 2,
        eye_y + eye_size
    ], fill=(0, 0, 0))
    # 右眼
    draw.ellipse([
        offset_x + base_size // 2 + head_size // 4 - eye_size // 2,
        eye_y,
        offset_x + base_size // 2 + head_size // 4 + eye_size // 2,
        eye_y + eye_size
    ], fill=(0, 0, 0))
    
    # 添加文字标签
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16)
    except:
        font = ImageFont.load_default()
    
    text = f"{habitat_name}\n{size_class_name}"
    # 使用textbbox获取文本边界
    bbox = draw.textbbox((0, 0), text, font=font)
    text_width = bbox[2] - bbox[0]
    text_height = bbox[3] - bbox[1]
    text_x = (SPRITE_SIZE[0] - text_width) // 2
    text_y = SPRITE_SIZE[1] - text_height - 10
    
    # 绘制文字背景
    draw.rectangle([
        text_x - 5,
        text_y - 5,
        text_x + text_width + 5,
        text_y + text_height + 5
    ], fill=(255, 255, 255, 200))
    
    # 绘制文字
    draw.text((text_x, text_y), text, fill=(0, 0, 0), font=font)
    
    return img


def create_part_sprite(part_name, color):
    """创建部件精灵"""
    img = Image.new('RGBA', SPRITE_SIZE, (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 根据部件类型绘制不同形状
    if "Wings" in part_name:
        # 翅膀：三角形
        points = [
            (SPRITE_SIZE[0] // 2, SPRITE_SIZE[1] // 2),
            (SPRITE_SIZE[0] - 20, 20),
            (SPRITE_SIZE[0] - 20, SPRITE_SIZE[1] - 20),
        ]
        draw.polygon(points, fill=color)
    elif "Tail" in part_name:
        # 尾巴：梯形
        points = [
            (20, SPRITE_SIZE[1] // 2 - 30),
            (SPRITE_SIZE[0] - 20, SPRITE_SIZE[1] // 2 - 10),
            (SPRITE_SIZE[0] - 20, SPRITE_SIZE[1] // 2 + 10),
            (20, SPRITE_SIZE[1] // 2 + 30),
        ]
        draw.polygon(points, fill=color)
    elif "Armor" in part_name or "Shell" in part_name:
        # 护甲：六边形
        center_x, center_y = SPRITE_SIZE[0] // 2, SPRITE_SIZE[1] // 2
        radius = 80
        points = []
        for i in range(6):
            angle = i * 60
            import math
            x = center_x + radius * math.cos(math.radians(angle))
            y = center_y + radius * math.sin(math.radians(angle))
            points.append((x, y))
        draw.polygon(points, fill=color)
    elif "Horns" in part_name:
        # 角：两个三角形
        # 左角
        points1 = [
            (SPRITE_SIZE[0] // 2 - 40, SPRITE_SIZE[1] // 2),
            (SPRITE_SIZE[0] // 2 - 60, 20),
            (SPRITE_SIZE[0] // 2 - 20, 20),
        ]
        draw.polygon(points1, fill=color)
        # 右角
        points2 = [
            (SPRITE_SIZE[0] // 2 + 40, SPRITE_SIZE[1] // 2),
            (SPRITE_SIZE[0] // 2 + 20, 20),
            (SPRITE_SIZE[0] // 2 + 60, 20),
        ]
        draw.polygon(points2, fill=color)
    elif "Spikes" in part_name:
        # 尖刺：多个小三角形
        for i in range(5):
            x = 30 + i * 40
            points = [
                (x, SPRITE_SIZE[1] // 2 - 40),
                (x - 15, SPRITE_SIZE[1] // 2),
                (x + 15, SPRITE_SIZE[1] // 2),
            ]
            draw.polygon(points, fill=color)
    elif "Bolt" in part_name:
        # 闪电：锯齿线
        points = [
            (SPRITE_SIZE[0] // 2, 20),
            (SPRITE_SIZE[0] // 2 + 30, SPRITE_SIZE[1] // 2 - 30),
            (SPRITE_SIZE[0] // 2, SPRITE_SIZE[1] // 2),
            (SPRITE_SIZE[0] // 2 + 30, SPRITE_SIZE[1] // 2 + 30),
            (SPRITE_SIZE[0] // 2, SPRITE_SIZE[1] - 20),
        ]
        draw.line(points, fill=color, width=10)
    elif "Cloak" in part_name:
        # 斗篷：半圆
        draw.pieslice([
            20, SPRITE_SIZE[1] // 2 - 80,
            SPRITE_SIZE[0] - 20, SPRITE_SIZE[1] // 2 + 80
        ], start=180, end=360, fill=color)
    elif "Aura" in part_name:
        # 光环：圆环
        draw.ellipse([40, 40, SPRITE_SIZE[0] - 40, SPRITE_SIZE[1] - 40], outline=color, width=15)
    else:
        # 默认：矩形
        draw.rectangle([60, 60, SPRITE_SIZE[0] - 60, SPRITE_SIZE[1] - 60], fill=color)
    
    # 添加文字标签
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16)
    except:
        font = ImageFont.load_default()
    
    bbox = draw.textbbox((0, 0), part_name, font=font)
    text_width = bbox[2] - bbox[0]
    text_height = bbox[3] - bbox[1]
    text_x = (SPRITE_SIZE[0] - text_width) // 2
    text_y = SPRITE_SIZE[1] - text_height - 10
    
    # 绘制文字背景
    draw.rectangle([
        text_x - 5,
        text_y - 5,
        text_x + text_width + 5,
        text_y + text_height + 5
    ], fill=(255, 255, 255, 200))
    
    # 绘制文字
    draw.text((text_x, text_y), part_name, fill=(0, 0, 0), font=font)
    
    return img


def main():
    """主函数"""
    print("开始生成临时测试精灵...")
    
    # 创建输出目录
    os.makedirs(SKELETON_DIR, exist_ok=True)
    os.makedirs(PART_DIR, exist_ok=True)
    
    # 生成骨架精灵
    print("\n生成骨架精灵...")
    skeleton_count = 0
    for habitat_name, color in HABITATS:
        for size_class_name, scale in SIZE_CLASSES:
            filename = f"S_{habitat_name}_{size_class_name}_Body.png"
            filepath = os.path.join(SKELETON_DIR, filename)
            
            img = create_skeleton_sprite(habitat_name, size_class_name, color, scale)
            img.save(filepath)
            
            skeleton_count += 1
            print(f"  [{skeleton_count}] {filename}")
    
    print(f"\n✅ 生成了 {skeleton_count} 个骨架精灵")
    
    # 生成部件精灵
    print("\n生成部件精灵...")
    part_count = 0
    for part_name, color in PARTS:
        filename = f"S_{part_name}.png"
        filepath = os.path.join(PART_DIR, filename)
        
        img = create_part_sprite(part_name, color)
        img.save(filepath)
        
        part_count += 1
        print(f"  [{part_count}] {filename}")
    
    print(f"\n✅ 生成了 {part_count} 个部件精灵")
    
    print(f"\n🎉 全部完成！")
    print(f"   骨架精灵: {SKELETON_DIR}")
    print(f"   部件精灵: {PART_DIR}")
    print(f"\n📝 下一步：")
    print(f"   1. 将这些精灵导入到UE4项目中")
    print(f"   2. 在UE4中创建Paper2D精灵资源")
    print(f"   3. 使用这些精灵测试怪兽生成系统")
    print(f"   4. 正式版本需要替换为美术设计的精灵")


if __name__ == "__main__":
    main()
