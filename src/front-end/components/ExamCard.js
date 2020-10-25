
import React, { useState, useEffect } from 'react'
import SWR from 'swr'
import { List, Avatar, Divider } from 'antd'
import { UserOutlined, MessageOutlined, LikeOutlined, StarOutlined } from '@ant-design/icons';
import * as mqtt from '../services/mqtt'

import '../styles/Home.module.less'



const ExamContent = (props) => {

  const { content = '{}' } = props

  const { GpenID, StuName, StuIDCardNo, CurrTime, Topic, Answer, Sign, PoEHash } = JSON.parse(content)

  return (
    <div className='_container'>
      <ul style={{width: '376px', padding: '10px 10px'}} ><span>实时答题:</span> 
        <Divider />
        <li><span>上链时间: </span>{ CurrTime } </li>
        <li><span>笔编号: </span>{ GpenID } </li>
        <li><span>当前题目: </span>{ Topic } </li>
        <li><span>答题: </span>{ Answer } </li>
        <li><span>数字签名: </span><span style={{wordWrap: 'break-word', fontSize:'12px'}}>{ Sign }</span> </li>
        <li><span>PoEHash: </span>{ PoEHash } </li>
      </ul>
      <style jsx>{`
        ._container {
          padding-right: 20px;
          border: 2px solid #fff7e6;
          min-height: 120px;
          display: flex;
          flex-direction: row;
        }
        ._container span {
          font-size: 14px;
          color: #595959;
          margin-right: 5px;
        }
        ._container li {
          margin-left: 10px;
        }
      `}</style>
    </div>
  )
}

const ExamCheck = (props) => {

  const { content = '{}' } = props

  const { GpenID, StuName, StuIDCardNo, CurrTime, StuPic, StuPicID, Sign, PoEHash, StuPicName } = JSON.parse(content)

  // http://172.25.0.231:9001/
  const picUrl = StuPicName && `${process.env.APIV1}${StuPicName}?token=${process.env.API_TOKEN}`.replace(/([^:]\/)\/+/g, "$1")

  return (
    <div className='_container'>
      <div style={{width: '400px', padding: '10px 10px'}} >
      <ul > <span>考生身份实时检测:</span>
        <Divider />
        <li><span>上链时间: </span>{ CurrTime } </li>
        <li><span>笔编号 : </span>{ GpenID } </li>
        <li><span>姓名: </span>{ StuName } </li>
        <li><span>数字签名:</span> <span style={{wordWrap: 'break-word', fontSize:'12px'}}>{ Sign }</span> </li>
        <li><span>PoEHash: </span>{ PoEHash } </li>
        </ul>
      </div>
      <div className='_img_box'>
        {
          picUrl ? (
            <img style={{width: '240px'}} src={picUrl} />
          ) : null
        }
      </div>
      <style jsx>{`
        ._container {
          padding-right: 5px;
          border: 2px solid #fff7e6;
          min-height: 120px;
          display: flex;
          flex-direction: row;

        }
        ._container span {
          font-size: 14px;
          color: #595959;
          margin-right: 5px;
        }
        ._container li {
          margin-left: 10px;
        }
        ._container span {
          font-size: 16px;
        }
        ._img_box {
          align-self: center;
          width: 176px, 
          height: 144px;
          border: 1 solid #eeeeee;
        }
      `}</style>
    </div>
  )
}

const ExamCard = (props) => {

  const { item } = props
  const { penId  } = item
  const url = `${process.env.APIV1}${item.phote.url}?token=${process.env.API_TOKEN}`.replace(/([^:]\/)\/+/g, "$1")

  const [content, setContent] = useState(item.content)
  const [check, setCheck] = useState()

  useEffect( () => {
    let mounted = true

    let mqttClientName = `exam_pen_${penId}` 
    let client = mqtt.createClient(mqttClientName, `/exam/${penId}/answer`, (topic, message) => {
      setContent('{}')
      setTimeout(() => setContent(message), 200);
    } )

    let client2 = mqtt.createClient(mqttClientName, `/exam/${penId}/check`, (topic, message) => {
      setCheck('{}')
      setTimeout(() => setCheck(message), 500);
    } )


    return () => {
      console.log(`${mqttClientName} exit`)
      //stop mqtt
      mqtt.closeClient(client)
      mqtt.closeClient(client2)
    } 
  }, [] )

  const avatar = (url) => url ? (<Avatar src={url} />)
    : ( <Avatar style={{background: '#87d068'}} icon={<UserOutlined /> } />)

  return (
    <>
    <List.Item
      key={item.title}
    >
      <List.Item.Meta
        avatar= { avatar(url) }
        title={<a href={url}>{item.title}</a>}
        description={item.description}
      />
    <div className='_container'>
      <ExamContent content={content} />
      <ExamCheck content={check} />
    </div>
    </List.Item>
    <style jsx>{`
    ._container {

      border: 1px solid #eeeeee;
      padding: 10px;
      min-height: 356px;
      display: flex;
      flex-direction: row;
    }
    `}</style>

    </>
  )
}

export default ExamCard

