import React, { useState, useEffect } from 'react'
import SWR from 'swr'
import { List, Breadcrumb, Space } from 'antd'
import qs from 'qs'
import { MessageOutlined, LikeOutlined, StarOutlined } from '@ant-design/icons';
import { usePagination, useRowSelection } from '../utils/hooks'
import ExamCard from '../components/ExamCard'
import '../styles/Home.module.less'


const Live = () => {

  const url = `${process.env.APIV1}exams?token=${process.env.API_TOKEN}`  // &${qs.stringify({page: currentPage, pageSize: 10})}
  const { data, error } = SWR( ['exam'  ], () => fetch(url, {
    method: 'GET',
    headers: {
      'Content-Type': 'application/json'
    }
  }).then( res => res.json() )
    .then( data => {
      return data
    } )
    .then( data => data.map ( (item, idx) => ( {

      title: `${item.candidate.name}`,
      penId: item.smartpen.GpenID,
      avatar: `${item.candidate.phote}`,
      phote: item.candidate.phote,
      description: `考试内容: ${item.title},  智能笔: ${item.smartpen.GpenID}`
    } ) ) )
  )

  const url1 = `${process.env.APIV1}exams/count?token=${process.env.API_TOKEN}` 
  const { currentPage, setCurrentPage, pagination } = usePagination('exams', url1)

  return (
    <>
      <main className='sub_main'>
        <h1 className='title'> 考场实时上链数据 </h1>
        <div style={{paddingLeft: '50px', width: '100%'}}>
          <Breadcrumb>
            <Breadcrumb.Item>
              <a href='/'>Home </a>
            </Breadcrumb.Item>
            <Breadcrumb.Item> Live Exam </Breadcrumb.Item>
          </Breadcrumb>
        </div>

      <div style={{padding: '20px 80px', width: '100%'}}>
        <List
          itemLayout="vertical"
          bordered={true}
          size="large"
          pagination={pagination}
          dataSource={data}
          split={true}
          renderItem={item => (
            <ExamCard item={item} />
          )}
        />
      </div>
      </main>
  </>
  )
}

export default  Live;
