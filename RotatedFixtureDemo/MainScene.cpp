/********************************************************************
 * File   : MainScene.cpp
 * Project: ToolsDemo
 *
 ********************************************************************
 * Created on 9/21/13 By Nonlinear Ideas Inc.
 * Copyright (c) 2013 Nonlinear Ideas Inc. All rights reserved.
 ********************************************************************
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 *    not claim that you wrote the original software. If you use this
 *    software in a product, an acknowledgment in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and
 *    must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 */

#include "MainScene.h"
#include "Box2DDebugDrawLayer.h"
#include "GridLayer.h"
#include "DebugLinesLayer.h"
#include "Notifier.h"
#include "Viewport.h"
#include "MathUtilities.h"


MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}


void MainScene::CreatePhysics()
{
   // Set up the viewport
   static const float32 worldSizeMeters = 10.0;
   
   // Initialize the Viewport
   Viewport::Instance().Init(worldSizeMeters);
   Vec2 gravity(0.0,0.0);   
   _world = new b2World(gravity);
   // Do we want to let bodies sleep?
   // No for now...makes the debug layer blink
   // which is annoying.
   _world->SetAllowSleeping(false);
   _world->SetContinuousPhysics(true);
}

bool MainScene::init()
{
   
   // Create physical world
   CreatePhysics();
   
   // Create the body with fixtures
   CreateBody();
   
   // Create the sprites
   CreateSprites();
   
   // Adding the debug lines so that we can draw the path followed.
   addChild(DebugLinesLayer::create());
   
   // Box2d Debug
   addChild(Box2DDebugDrawLayer::create(_world));
   
   // Grid
   addChild(GridLayer::create());
   
   return true;
}

Vec2 CalculateAverage(vector<Vec2>& points)
{
   Vec2 sum(0,0);
   for(int idx = 0; idx < points.size(); idx++)
   {
      sum += points[idx];
   }
   if(points.size() > 0)
      sum *= 1.0/(points.size());
   return sum;
}

void MainScene::CreateBody()
{
   Vec2 position(0,0);
   
   // Create the body.
   b2BodyDef bodyDef;
   bodyDef.position = position;
   bodyDef.type = b2_dynamicBody;
   _body = _world->CreateBody(&bodyDef);
   assert(_body != NULL);
   
   // Now attach fixtures to the body.
   FixtureDef fixtureDef;
   PolygonShape polyShape;
   vector<Vec2> vertices;
   
   const float32 VERT_SCALE = .5;
   fixtureDef.shape = &polyShape;
   fixtureDef.density = 1.0;
   fixtureDef.friction = 1.0;
   fixtureDef.isSensor = false;
   
   // Main Box
   vertices.clear();
   vertices.push_back(Vec2(1*VERT_SCALE,1*VERT_SCALE));
   vertices.push_back(Vec2(-1*VERT_SCALE,1*VERT_SCALE));
   vertices.push_back(Vec2(-1*VERT_SCALE,-1*VERT_SCALE));
   vertices.push_back(Vec2(1*VERT_SCALE,-1*VERT_SCALE));
   polyShape.Set(&vertices[0],vertices.size());
   _body->CreateFixture(&fixtureDef);
   _fixturePositions.push_back(CalculateAverage(vertices));
   
   // Down one
   vertices.clear();
   vertices.push_back(Vec2(1*VERT_SCALE,-1*VERT_SCALE));
   vertices.push_back(Vec2(-1*VERT_SCALE,-1*VERT_SCALE));
   vertices.push_back(Vec2(-1*VERT_SCALE,-3*VERT_SCALE));
   vertices.push_back(Vec2(1*VERT_SCALE,-3*VERT_SCALE));
   polyShape.Set(&vertices[0],vertices.size());
   _body->CreateFixture(&fixtureDef);
   _fixturePositions.push_back(CalculateAverage(vertices));
   
   // Up One
   vertices.clear();
   vertices.push_back(Vec2(1*VERT_SCALE,3*VERT_SCALE));
   vertices.push_back(Vec2(-1*VERT_SCALE,3*VERT_SCALE));
   vertices.push_back(Vec2(-1*VERT_SCALE,1*VERT_SCALE));
   vertices.push_back(Vec2(1*VERT_SCALE,1*VERT_SCALE));
   polyShape.Set(&vertices[0],vertices.size());
   _body->CreateFixture(&fixtureDef);
   _fixturePositions.push_back(CalculateAverage(vertices));
   
   // T Left Top
   vertices.clear();
   vertices.push_back(Vec2(-1*VERT_SCALE,3*VERT_SCALE));
   vertices.push_back(Vec2(-3*VERT_SCALE,3*VERT_SCALE));
   vertices.push_back(Vec2(-3*VERT_SCALE,1*VERT_SCALE));
   vertices.push_back(Vec2(-1*VERT_SCALE,1*VERT_SCALE));
   polyShape.Set(&vertices[0],vertices.size());
   _body->CreateFixture(&fixtureDef);
   _fixturePositions.push_back(CalculateAverage(vertices));
   
   // T Right Top
   vertices.clear();
   vertices.push_back(Vec2(3*VERT_SCALE,3*VERT_SCALE));
   vertices.push_back(Vec2(1*VERT_SCALE,3*VERT_SCALE));
   vertices.push_back(Vec2(1*VERT_SCALE,1*VERT_SCALE));
   vertices.push_back(Vec2(3*VERT_SCALE,1*VERT_SCALE));
   polyShape.Set(&vertices[0],vertices.size());
   _body->CreateFixture(&fixtureDef);
   _fixturePositions.push_back(CalculateAverage(vertices));
   
   _body->SetAngularVelocity(M_PI/8);
}

void MainScene::UpdateBody()
{
   
}

MainScene* MainScene::create()
{
   MainScene *pRet = new MainScene();
   if (pRet && pRet->init())
   {
      pRet->autorelease();
      return pRet;
   }
   else
   {
      CC_SAFE_DELETE(pRet);
      return NULL;
   }
}

void MainScene::onEnter()
{
   CCScene::onEnter();
}

void MainScene::onExit()
{
   CCScene::onExit();
}

void MainScene::onEnterTransitionDidFinish()
{
   CCScene::onEnterTransitionDidFinish();
   Viewport::Instance().SetScale(1.0);
   // Schedule Updates
   scheduleUpdate();
}

void MainScene::onExitTransitionDidStart()
{
   CCScene::onExitTransitionDidStart();
   
   // Turn off updates
   unscheduleUpdate();
}

void MainScene::UpdateSprites()
{
   for(int idx = 0; idx < _fixturePositions.size(); idx++)
   {
      CCPoint spritePosition = Viewport::Instance().Convert(_body->GetWorldPoint(_fixturePositions[idx]));
      _fixtureSprites[idx]->setPosition(spritePosition);
      float32 bodyAngle = _body->GetAngle();
      bodyAngle = MathUtilities::AdjustAngle(bodyAngle);
      _fixtureSprites[idx]->setRotation(-CC_RADIANS_TO_DEGREES(bodyAngle));
   }
}

void MainScene::CreateSprites()
{
   Viewport& vp = Viewport::Instance();
   
   for(int idx = 0; idx < _fixturePositions.size(); idx++)
   {
      CCSprite* sprite = CCSprite::create("arrow.png");
      CCLOG("Viewport PTM Ratio = %f",vp.GetPTMRatio());
      sprite->setScale(1.0*vp.GetPTMRatio()/128);
      _fixtureSprites.push_back(sprite);
      addChild(sprite);
   }
}


void MainScene::UpdatePhysics()
{
   const int velocityIterations = 8;
   const int positionIterations = 1;
   float32 fixedDT = SECONDS_PER_TICK;
   // Instruct the world to perform a single step of simulation. It is
   // generally best to keep the time step and iterations fixed.
   _world->Step(fixedDT, velocityIterations, positionIterations);
}

void MainScene::update(float dt)
{
   UpdatePhysics();
   UpdateBody();
   UpdateSprites();
}

