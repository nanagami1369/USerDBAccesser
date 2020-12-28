<template>
  <table id="user-data-table">
    <thead>
      <th>ID</th>
      <th>名前</th>
      <th>パスワード</th>
      <th>状態</th>
      <th>権限</th>
    </thead>
    <tbody>
      <tr v-for="user in userInfo" :key="user.id">
        <td>{{ user.id }}</td>
        <td>{{ user.name }}</td>
        <td>{{ user.pass }}</td>
        <td>{{ user.avail | availToString }}</td>
        <td>{{ user.level }}</td>
      </tr>
    </tbody>
  </table>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator'
import { User } from '@/model/User'

@Component({
  filters: {
    availToString: function(avail: boolean) {
      if (avail == true) {
        return '有効'
      } else {
        return '無効'
      }
    }
  }
})
export default class UserInfoTable extends Vue {
  @Prop() private userInfo!: User[]
}
</script>
